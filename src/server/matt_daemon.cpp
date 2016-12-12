/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 20:27:56 by root              #+#    #+#             */
/*   Updated: 2016/12/12 02:03:46 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include "matt_daemon.hpp"
#include "Server.hpp"
#include "Tintin_reporter.hpp"

static void	sigHandler(int s) __attribute__((noreturn));
static void	sigHandler(int s)
{
	Tintin_reporter::LogInfo("Signal(" + std::to_string(s)  + ") " + std::string(strsignal(s)) + " catched");
	Tintin_reporter::LogInfo("Quitting");
	unlink(LOCK_FILE);
	Tintin_reporter::DeInit();
	exit(0);
}

static void	work(void)
{
	Server				server;
	Tintin_reporter::LogInfo("Starting is now listening on port 4242");

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
			(void)sock;
		}
	);

	server.LoopUntilQuit();
}

extern "C" void		matt_daemon(int lock_fd)
{
	int		pid_fd;
	pid_t	pid;
	Tintin_reporter::Init();

	for (int i = 0; i < 32; i++)
		signal(i, sigHandler);
	signal(SIGCHLD, SIG_IGN);

	umask(0);

	chdir("/");

	setsid();
	Tintin_reporter::LogInfo("Starting daemon at pid " + std::to_string(getpid()));
 	pid_fd = open(PID_FILE, O_WRONLY | O_CREAT, 0755);
	pid = getpid();
	write(pid_fd, &pid, sizeof(int));
	close(pid_fd);

	work();

	Tintin_reporter::LogInfo("Quitting");
	flock(lock_fd, LOCK_UN);
	close(lock_fd);
	Tintin_reporter::DeInit();
	exit(0);
}
