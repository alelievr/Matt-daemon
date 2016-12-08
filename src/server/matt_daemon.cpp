/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 20:27:56 by root              #+#    #+#             */
/*   Updated: 2016/12/08 21:13:22 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt_daemon.hpp"
#include <string>
#include "Server.hpp"

static void	work(void)
{
	Server	server;
	int		nClients = 0;

	server.setOnNewClientConnected([&](const std::string & ip) {
			std::cout << "new client connected: " << ip << std::endl;
			nClients++;
		}
	);
	server.setOnClientDisconnected([&](const std::string & ip) {
			std::cout << "client disconnected: " << ip << std::endl;
		}
	);
	server.setOnClientRead([&](const std::string & ip, std::string & message) {
			std::cout << "new message from: " << ip  << message << std::endl;
		}
	);

	server.LoopUntilQuit();
}

extern "C" void		matt_daemon(int lock_fd)
{
	int		pid_fd = open(PID_FILE, O_WRONLY | O_CREAT, 0755);
	int		pid;

	umask(0);

	chdir("/");

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	setsid();

	pid = getpid();
	write(pid_fd, &pid, sizeof(int));
	close(pid_fd);

	work();

	flock(lock_fd, LOCK_UN);
	close(lock_fd);
	exit(0);
}
