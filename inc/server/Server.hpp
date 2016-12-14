/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:09:04 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/14 01:26:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include <sys/select.h>
#include <string.h>
#include <termios.h>
#include "RSAEncrypt.hpp"
#include "Tintin_reporter.hpp"
#include "globals.h"

#include <map>
#include <iostream>
#include <string>

typedef struct
{
	std::string	ip;
	int			clientNumber;
	pid_t		shellPid;
	int			master;
	int			:32;
}				Client;

#define		NEW_CLIENT(ip)	Client{ip, -1, 0, 0}
#define		WRITE			1
#define		READ			0

class		Server
{
	private:
		int															_socket;
		int															_port;
		int															_connectedClientsNumber;
		bool														_quit;
		long														:24;
		std::function< void(const Client &, bool accepted) >	_onNewClientConnected;
		std::function< void(const Client &, const int sock, std::string &) >	_onClientRead;
		std::function< void(const Client &) >					_onClientDisconnected;
		//map with socket in key and ip as value.
		std::map< int, Client >										_connectedClients;
		struct winsize *											_window;
		struct termios *											_terminal;

		void	openSocket(const int port);
		void	NewConnection(const int sock, fd_set *fds);
		void	ReadFromClient(const int sock, fd_set *fds);
		void	ReadFromShell(const int shellStdout, const int clientSock, fd_set *fds);
		void	DisconnectClient(const int sock, fd_set *fds);

	public:
		Server(void);
		Server(int port);
		Server(int port, struct termios *term, struct winsize *win);
		Server(const Server&) = delete;
		virtual ~Server(void);

		Server &	operator=(Server const & src) = delete;

		void	LoopUntilQuit(void);
		void	WriteToClient(int id, char *msg, size_t size);

		void	setOnNewClientConnected(std::function< void(const Client &, bool accepted) > tmp);
		void	setOnClientRead(std::function< void(const Client &, const int sock, std::string &) > tmp);
		void	setOnClientDisconnected(std::function< void(const Client &) > tmp);
};

std::ostream &	operator<<(std::ostream & o, Server const & r);
