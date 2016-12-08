/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:10:29 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 16:26:48 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	listen(_socket, MAX_CONNECTION_QUEUE);
}

Server::Server(void)
{
	_port = 4242;
	openSocket(_port);
}

Server::Server(int p) : _port(p)
{
	std::cout << "Default constructor of Server called" << std::endl;
	this->_onNewClientConnected = NULL;
	this->_onClientRead = NULL;
	this->_onClientDisconnected = NULL;
	openSocket(_port);
}

Server::~Server(void)
{
	std::cout << "Destructor of Server called" << std::endl;
}

void		Server::setOnNewClientConnected(std::function< void(const std::string &) > tmp) { this->_onNewClientConnected = tmp; }
void		Server::setOnClientRead(std::function< void(const std::string &, std::string &) > tmp) { this->_onClientRead = tmp; }
void		Server::setOnClientDisconnected(std::function< void(const std::string &) > tmp) { this->_onClientDisconnected = tmp; }

std::ostream &	operator<<(std::ostream & o, Server const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
