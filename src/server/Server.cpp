/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:10:29 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/09 01:34:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <arpa/inet.h>

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
	_connectedClientsNumber++;
	std::string ip = inet_ntoa(client_name.sin_addr);
	if (_onNewClientConnected != NULL)
		_onNewClientConnected(ip, true);
	Client	c = NEW_CLIENT(ip);
	_connectedClients[new_sock] = c;
	FD_SET(new_sock, fds);
}

void	Server::ReadFromClient(const int sock, fd_set *fds)
{
	char				buff[0xF00];
	std::string			stdbuff;
	long				r;

	if ((r = read(sock, buff, sizeof(buff) - 1)) <= 0)
	{
		if (_onClientDisconnected != NULL)
			_onClientDisconnected(_connectedClients[sock].ip);
		_connectedClientsNumber--;
		close(sock);
		FD_CLR(sock, fds);
	}
	else
	{
		buff[r] = 0;
		stdbuff = std::string(buff);
		if (stdbuff == "quit")
			_quit = true;
		if (_onClientRead != NULL)
			_onClientRead(_connectedClients[sock].ip, stdbuff);
	}
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
				if (i == _socket)
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
void		Server::setOnClientRead(std::function< void(const std::string &, std::string &) > tmp) { this->_onClientRead = tmp; }
void		Server::setOnClientDisconnected(std::function< void(const std::string &) > tmp) { this->_onClientDisconnected = tmp; }

std::ostream &	operator<<(std::ostream & o, Server const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
