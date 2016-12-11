/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:10:29 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/11 20:58:16 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pty.h>

void	Server::openSocket(int port)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;
	int					activate = 1;

	proto = getprotobyname("tcp");
	if ((_socket = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		perror("socket"), exit(-1);
	endprotoent();
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(int)) < 0)
		perror("setsockopt"), exit(-1);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_socket, reinterpret_cast< const struct sockaddr * >(&sin), sizeof(sin)) == -1)
		perror("bind"), exit(-1);
	if (listen(_socket, MAX_CONNECTION_QUEUE) < 0)
		perror("listen"), exit(-1);
}

Server::Server(void) : _connectedClientsNumber(0), _quit(false)
{
	_port = 4242;
	openSocket(_port);
}

Server::Server(int p) : _port(p), _connectedClientsNumber(0), _quit(false)
{
	std::cout << "Default constructor of Server called" << std::endl;
	this->_onNewClientConnected = NULL;
	this->_onClientRead = NULL;
	this->_onClientDisconnected = NULL;
	openSocket(_port);
}

void	Server::NewConnection(const int sock, fd_set *fds)
{
	int					new_sock;
	struct sockaddr_in	client_name;
	socklen_t			size;
//	int					peer_len;

	size = sizeof(client_name);
	if ((new_sock = accept(sock, reinterpret_cast< struct sockaddr * >(&client_name), &size)) < 0)
	{
		perror("accept");
		return ;
	}

	if (_connectedClientsNumber >= 3)
	{
		_onNewClientConnected("", false);
		close(new_sock);
		return ;
	}

//	getpeername(new_sock, &client_name, &peer_len);
	std::string ip = inet_ntoa(client_name.sin_addr);
	if (_onNewClientConnected != NULL)
		_onNewClientConnected(ip, true);

	Client	c = NEW_CLIENT(ip);

	//open shell and pipes
	if (pipe(c.inPipe) == -1
			|| pipe(c.outPipe) == -1
			|| pipe(c.errPipe) == -1
			|| (c.shellPid = forkpty(&c.master, NULL, NULL, NULL)) == -1)
	{
		close(new_sock);
		return ;
	}
	if (c.shellPid == 0)
	{
		dup2(STDOUT_FILENO, STDERR_FILENO);
		close(c.inPipe[WRITE]);
		dup2(c.inPipe[READ], STDIN_FILENO);
		close(c.inPipe[READ]);
		close(c.outPipe[READ]);
		dup2(c.outPipe[WRITE], STDOUT_FILENO);
		dup2(c.errPipe[WRITE], STDERR_FILENO);
		close(c.outPipe[WRITE]);
		exit(execl("/bin/bash", "bash", NULL));
	}
	else
	{
		close(c.inPipe[READ]);
		close(c.outPipe[WRITE]);
		close(c.errPipe[WRITE]);

		//binding the shell standard output to the select
		FD_SET(c.outPipe[READ], fds);
		FD_SET(c.errPipe[READ], fds);
	}

	_connectedClients[new_sock] = c;

	FD_SET(new_sock, fds);
	_connectedClientsNumber++;
}

void	Server::ReadFromClient(const int sock, fd_set *fds)
{
	char				buff[0xF00];
	std::string			stdbuff;
	long				r;

	if ((r = read(sock, buff, sizeof(buff) - 1)) <= 0)
	{
		Client c = _connectedClients[sock];
		if (_onClientDisconnected != NULL)
			_onClientDisconnected(c.ip);
		kill(SIGKILL, c.shellPid);
		close(c.inPipe[READ]);
		close(c.inPipe[WRITE]);
		close(c.outPipe[READ]);
		close(c.outPipe[WRITE]);
		close(c.errPipe[READ]);
		close(c.errPipe[WRITE]);
		//close the client pty
		close(c.master);
		_connectedClientsNumber--;
		close(sock);
		FD_CLR(sock, fds);
		FD_CLR(c.outPipe[READ], fds);
		FD_CLR(c.errPipe[READ], fds);
	}
	else
	{
		buff[r] = 0;
		stdbuff = std::string(buff);
		_rsa.Decode(stdbuff);
		Client c = _connectedClients[sock];
		write(c.inPipe[WRITE], stdbuff.c_str(), static_cast< size_t >(r));
		if (stdbuff == "quit")
			_quit = true;
		if (_onClientRead != NULL)
			_onClientRead(c.ip, sock, stdbuff);

	}
}

void	Server::ReadFromShell(const int shellStdout, const int clientSock, bool isStdout)
{
	char			buff[0xF000];
	std::string		stdbuff;
	long			r;

	if ((r = read(shellStdout, buff, sizeof(buff) - 1)) == -1)
	{
		Tintin_reporter::LogError("can't read on shell's out pipe.");
		return ;
	}
	buff[r] = 0;
	stdbuff = std::to_string(static_cast< int >(isStdout)) + std::string(buff);
	WriteToClient(clientSock, stdbuff);
}

void	Server::WriteToClient(const int sock, std::string & message)
{
	_rsa.Encode(message);
	write(sock, message.c_str(), message.size());
}

void	Server::LoopUntilQuit(void)
{
	fd_set					read_fds;
	fd_set					active_fds;

	FD_ZERO(&active_fds);
	FD_SET(_socket, &active_fds);
	while (42)
	{
		read_fds = active_fds;
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) < 0)
			perror("select"), exit(-1);

		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_fds))
			{
				bool	readedFromShell = false;

				//check if to read fd is a shell stdout and if yes, send result to connected client
				for (const auto & c : _connectedClients)
					if (i == c.second.outPipe[READ])
						ReadFromShell(i, c.first, true), readedFromShell = true;
					else if (i == c.second.errPipe[READ])
						ReadFromShell(i, c.first, false), readedFromShell = true;
				if (readedFromShell)
					;
				else if (i == _socket)
					NewConnection(i, &active_fds);
				else
					ReadFromClient(i, &active_fds);
			}
		if (_quit)
			break ;
	}
}

Server::~Server(void)
{
}

void		Server::setOnNewClientConnected(std::function< void(const std::string &, bool accepted) > tmp) { this->_onNewClientConnected = tmp; }
void		Server::setOnClientRead(std::function< void(const std::string &, int sock, std::string &) > tmp) { this->_onClientRead = tmp; }
void		Server::setOnClientDisconnected(std::function< void(const std::string &) > tmp) { this->_onClientDisconnected = tmp; }

std::ostream &	operator<<(std::ostream & o, Server const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
