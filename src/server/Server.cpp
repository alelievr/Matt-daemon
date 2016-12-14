/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:10:29 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/14 03:59:15 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pty.h>
#include <poll.h>

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
	sin.sin_port = htons(static_cast< uint16_t >(port));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_socket, reinterpret_cast< const struct sockaddr * >(&sin), sizeof(sin)) == -1)
		perror("bind"), exit(-1);
	if (listen(_socket, MAX_CONNECTION_QUEUE) < 0)
		perror("listen"), exit(-1);
}

Server::Server(void) : Server(4242)
{
}

Server::Server(int p) : Server(p, NULL, NULL)
{
}

Server::Server(int p, struct termios *term, struct winsize *win) :
	_port(p),
	_connectedClientsNumber(0),
	_quit(false),
	_onNewClientConnected(NULL),
	_onClientRead(NULL),
	_onClientDisconnected(NULL),
	_window(win),
	_terminal(term)
{
	openSocket(_port);
}

void	Server::NewConnection(const int sock, fd_set *fds)
{
	int					new_sock;
	struct sockaddr_in	client_name;
	socklen_t			size;
	bool				ok;
//	int					peer_len;

	size = sizeof(client_name);
	if ((new_sock = accept(sock, reinterpret_cast< struct sockaddr * >(&client_name), &size)) < 0)
	{
		perror("accept");
		return ;
	}

	if (_connectedClientsNumber >= 3)
	{
		_onNewClientConnected(NEW_CLIENT(""), false);
		close(new_sock);
		return ;
	}

//	getpeername(new_sock, &client_name, &peer_len);
	std::string ip = inet_ntoa(client_name.sin_addr);

	Client	c = NEW_CLIENT(ip);

	//generate client number (between 0 and 3)
	c.clientNumber = 0;
	while (42)
	{
		ok = true;
		for (const auto cc : _connectedClients)
			if (cc.second.clientNumber == c.clientNumber)
			{
				c.clientNumber++;
				ok = false;
				break ;
			}
		if (ok)
			break ;
	}

	//open shell and pipes
	if ((c.shellPid = forkpty(&c.master, NULL, _terminal, _window)) == -1)
	{
		close(new_sock);
		return ;
	}
	if (c.shellPid == 0)
		exit(execl("/bin/zsh", "zsh", NULL));

	if (_onNewClientConnected != NULL)
		_onNewClientConnected(c, true);

	FD_SET(c.master, fds);

	_connectedClients[new_sock] = c;

	FD_SET(new_sock, fds);
	_connectedClientsNumber++;
}

void	Server::DisconnectClient(const int sock, fd_set *fds)
{
	Client c = _connectedClients[sock];
	if (_onClientDisconnected != NULL)
		_onClientDisconnected(c);
	kill(SIGKILL, c.shellPid);

	//close the client pty
	close(c.master);

	_connectedClientsNumber--;

	//close and remove client socket
	close(sock);
	FD_CLR(sock, fds);
	FD_CLR(c.master, fds);
	_connectedClients.erase(sock);
}

void	Server::ReadFromClient(const int sock, fd_set *fds)
{
	std::string			stdbuff;
	long				r;

	if ((stdbuff = RSAEncrypt::ReadOn(sock, &r)).empty())
		DisconnectClient(sock, fds);
	else
	{
		Client c = _connectedClients[sock];
		if (stdbuff[0] == static_cast< char >('\x80'))
		{
			stdbuff.erase(0, 1);
			int sig = std::stoi(stdbuff);
			killpg(getpgid(c.shellPid), sig);
			Tintin_reporter::LogInfo("client [" + c.ip + "] has sent a signal to remote shell: \"" + strsignal(sig) + "\"");
		}
		else
		{
 		   	if (_onClientRead != NULL)
				_onClientRead(c, sock, stdbuff);
			write(c.master, stdbuff.c_str(), stdbuff.size());
		}
	}
}

void	Server::ReadFromShell(const int shellTTY, const int clientSock, fd_set *fds)
{
	char			buff[0xF000];
	long			r = 1;

	Client c = _connectedClients[clientSock];
	if ((r = read(shellTTY, buff, sizeof(buff) - 1)) == -1)
	{
		Tintin_reporter::LogError("client [" + c.ip + "]: can't read on shell's master TTY, disconnecting client");
		DisconnectClient(clientSock, fds);
		return ;
	}
	if (r > 0)
	{
		buff[r] = 0;
		Tintin_reporter::LogClient(c.clientNumber, "client [" + c.ip + "]: shell result: " + buff);
		WriteToClient(clientSock, buff, static_cast< size_t >(r));
	}
	else
	{
		Tintin_reporter::LogInfo("client [" + c.ip + "]: shell's master TTY closed, disconnecting client");
		DisconnectClient(clientSock, fds);
	}
}

void	Server::WriteToClient(const int sock, char *msg, size_t size)
{
	RSAEncrypt::WriteTo(sock, msg, size);
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
			Tintin_reporter::LogError(std::string("select: ") + strerror(errno)), raise(SIGQUIT);

		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_fds))
			{
				bool	readedFromShell = false;

				//check if to read fd is a shell stdout and if yes, send result to connected client
				for (const auto & c : _connectedClients)
					if (i == c.second.master)
						ReadFromShell(i, c.first, &active_fds), readedFromShell = true;
				if (readedFromShell)
					std::cout << "readed from shell\n";
				else if (i == _socket)
				{
					NewConnection(i, &active_fds);
					std::cout << "new connection\n";
				}
				else
				{
					ReadFromClient(i, &active_fds);
					std::cout << "readed on client\n";
				}
			}
		if (_quit)
			break ;
	}
}

Server::~Server(void)
{
}

void		Server::setOnNewClientConnected(std::function< void(const Client &, bool accepted) > tmp) { this->_onNewClientConnected = tmp; }
void		Server::setOnClientRead(std::function< void(const Client &, int sock, std::string &) > tmp) { this->_onClientRead = tmp; }
void		Server::setOnClientDisconnected(std::function< void(const Client &) > tmp) { this->_onClientDisconnected = tmp; }

std::ostream &	operator<<(std::ostream & o, Server const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
