/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:09:04 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 20:49:32 by root             ###   ########.fr       */
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

#define MAX_CONNECTION_QUEUE 3

class		Server
{
	private:
		int															_socket;
		int															_port;
		std::function< void(const std::string &) >					_onNewClientConnected;
		std::function< void(const std::string &, std::string &) >	_onClientRead;
		std::function< void(const std::string &) >					_onClientDisconnected;

		void	openSocket(const int port);
		void	NewConnection(const int sock, fd_set *fds) const;
		void	ReadFromClient(const int sock, fd_set *fds) const;

	public:
		Server(void);
		Server(int port);
		Server(const Server&) = delete;
		virtual ~Server(void);

		Server &	operator=(Server const & src) = delete;

		void	LoopUntilQuit(void);

		void	setOnNewClientConnected(std::function< void(const std::string &) > tmp);
		void	setOnClientRead(std::function< void(const std::string &, std::string &) > tmp);
		void	setOnClientDisconnected(std::function< void(const std::string &) > tmp);
};

std::ostream &	operator<<(std::ostream & o, Server const & r);
