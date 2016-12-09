/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:09:04 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/09 03:04:14 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"

#include <iostream>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include <sys/select.h>
#include <map>
#include "RSA.hpp"
#include "globals.h"

typedef struct
{
	std::string	ip;
	int			inPipe[2];	//shell pipe
	int			outPipe[2];	//shell pipe
}				Client;

#define		NEW_CLIENT(ip) Client{ip, {0, 0}, {0, 0}}

class		Server
{
	private:
		int															_socket;
		int															_port;
		int															_connectedClientsNumber;
		bool														_quit;
		long														:24;
		std::function< void(const std::string &, bool accepted) >	_onNewClientConnected;
		std::function< void(const std::string &, const int sock, std::string &) >	_onClientRead;
		std::function< void(const std::string &) >					_onClientDisconnected;
		//map with socket in key and ip as value.
		std::map< int, Client >										_connectedClients;
		RSA															_rsa;

		void	openSocket(const int port);
		void	NewConnection(const int sock, fd_set *fds);
		void	ReadFromClient(const int sock, fd_set *fds);

	public:
		Server(void);
		Server(int port);
		Server(const Server&) = delete;
		virtual ~Server(void);

		Server &	operator=(Server const & src) = delete;

		void	LoopUntilQuit(void);
		void	WriteToClient(int id, const std::string & message);

		void	setOnNewClientConnected(std::function< void(const std::string &, bool accepted) > tmp);
		void	setOnClientRead(std::function< void(const std::string &, const int sock, std::string &) > tmp);
		void	setOnClientDisconnected(std::function< void(const std::string &) > tmp);
};

std::ostream &	operator<<(std::ostream & o, Server const & r);
