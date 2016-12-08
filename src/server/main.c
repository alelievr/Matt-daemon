/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 12:47:24 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 20:25:30 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt_daemon.h"
#include <ctype.h>
#include <errno.h>
#include <string.h>

static void	flock_error(int lock_fd, int err)
{
	int		fd = open(PID_FILE, O_RDONLY);
	int		pid = 0;
	int		r;

	if (read(fd, &pid, 4) <= 0 || pid == 0)
		printf("flock: %s\n", strerror(err)), exit(-1);
	close(fd);
	if (err == 11)
	{
		printf("flock: %s\n", strerror(err));
		printf("another matt daemon is running at pid: %i\n", pid);
		printf("do you want to kill it ? (y/n) ");
		r = getchar();
		if (r == '\n' || r == 'y' || r == 'Y')
		{
			kill(pid, SIGKILL);
			flock(lock_fd, LOCK_UN);
			flock(lock_fd, LOCK_EX);
		}
		else
			exit(0);
	}
}

static void	help(const char *self) __attribute__((noreturn));
static void	help(const char *self)
{
	printf("%s options:\n -h for help\n -b to see the bonus list\n", self);
	exit(0);
}

static void	bonus(void) __attribute__((noreturn));
static void	bonus(void)
{
	printf("- advanced error dispay and management\n");
	printf("- remote shell\n");
	printf("- custom client with prompt\n");
	printf("- advanced logging\n");
	printf("- custom RSA based communication encryption\n");
	exit(0);
}

static void	options(int ac, char **av)
{
	int			c;
	const char	*self = av[0];

	while ((c = getopt(ac, av, "hb:")) != -1)
		if (c == 'h')
			help(self);
		else if (c == 'b')
			bonus();
}

int			main(int ac, char **av)
{
	pid_t		daemon_pid;
	uid_t		uid;
	int			lock_fd;

	options(ac, av);
	uid = getuid();
	if (uid != geteuid() || uid != 0)
		printf("%s must be run as root\n", av[0]), exit(-1);
	if ((lock_fd = open(LOCK_FILE, O_RDONLY | O_CREAT, 0755)) < 0)
		perror("open"), exit(-1);
	if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0)
		flock_error(lock_fd, errno);
	if ((daemon_pid = fork()) == -1)
		perror("fork"), exit(-1);
	if (daemon_pid == 0)
		matt_daemon(lock_fd);
	if (daemon_pid > 0)
		printf("daemon started\n");
	(void)ac;
	return (0);
}
