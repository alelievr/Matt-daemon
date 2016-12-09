/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 20:27:56 by root              #+#    #+#             */
/*   Updated: 2016/12/09 03:05:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <string.h>
#include <string>
#include "matt_daemon.hpp"
#include "Server.hpp"
#include "Tintin_reporter.hpp"

static void	sigHandler(int s) __attribute__((noreturn));
static void	sigHandler(int s)
{
	Tintin_reporter::Log("Signal " + std::string(strsignal(s)) + " catched");
	Tintin_reporter::Log("Quitting");
	unlink(LOCK_FILE);
	Tintin_reporter::DeInit();
	exit(0);
}

static void	work(void)
{
	Server				server;

	server.setOnNewClientConnected([&](const std::string & ip, bool accepted) {
			if (accepted)
				Tintin_reporter::LogInfo("New client connected: " + ip);
			else
				Tintin_reporter::LogError("Connection refused, max simultaneous connection reached");
		}
	);
	server.setOnClientDisconnected([&](const std::string & ip) {
			Tintin_reporter::LogInfo("Client disconnected: " + ip);
		}
	);
	server.setOnClientRead([&](const std::string & ip, int sock, std::string & message) {
			Tintin_reporter::Log("Client [" + ip + "]: " + message);
			//transfert message to shell
			server.WriteToClient(sock, message);
		}
	);

	Tintin_reporter::Log("Starting daemon at pid " + std::to_string(getpid()));
	server.LoopUntilQuit();
}

extern "C" void		matt_daemon(int lock_fd)
{
	int		pid_fd = open(PID_FILE, O_WRONLY | O_CREAT, 0755);
	int		pid;
	Tintin_reporter::Init();

	for (int i = 0; i < 32; i++)
		signal(i, sigHandler);

	umask(0);

	chdir("/");

//	close(STDIN_FILENO);
//	close(STDOUT_FILENO);
//	close(STDERR_FILENO);

	setsid();

	pid = getpid();
	write(pid_fd, &pid, sizeof(int));
	close(pid_fd);

	work();

	Tintin_reporter::Log("Quitting");
	flock(lock_fd, LOCK_UN);
	close(lock_fd);
	Tintin_reporter::DeInit();
	exit(0);
}
