/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:30:03 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/13 15:04:12 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ben_afk.hpp"

static int	currentServerSocket;

static void	stdin_event(int server_socket, const RSA & rsa)
{
	char		buff[0xF00];
	long		ret;
	std::string	message;

	if ((ret = read(STDIN_FILENO, buff, sizeof(buff) -1 )) < 0)
		close(server_socket), exit(-1);
	for (int i = 0; i < ret; i++)
	{
	//	if ((isprint(buff[i]) || buff[i] == '\n') && buff[i] != '\t')
	//		write(STDOUT_FILENO, buff + i, 1);
	}
	if (ret == 0)
		printf("quitting client ...\n"), exit(0);
	buff[ret] = 0;
	message = std::string(buff);
	rsa.EncodeWrite(server_socket, message);
}

static void server_event(int server_socket, const RSA & rsa)
{
	long		ret;
	std::string	message;

	if ((message = rsa.DecodeRead(server_socket, &ret)).empty())
		close(server_socket), puts("closing connection ..."), exit(-1);
	if (ret == 0)
		printf("server closed the connection !\n"), exit(0);
	std::cout << message << std::flush;
}

static void	client_io(int server_socket) __attribute__((noreturn));
static void	client_io(int server_socket)
{
	fd_set		read_fds;
	fd_set		active_fds;
	RSA			rsa;

	FD_ZERO(&read_fds);
	FD_SET(STDIN_FILENO, &read_fds);
	FD_SET(server_socket, &read_fds);
	active_fds = read_fds;
	while (42)
	{
		read_fds = active_fds;
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) < 0)
		{
			if (errno == 4)
				continue;
			close(server_socket), perror("select"), exit(-1);
		}
		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_fds))
			{
				if (i == 0)
					stdin_event(server_socket, rsa);
				else if (i == server_socket)
					server_event(server_socket, rsa);
				else
					close(i);
			}
	}
}

static int	connect_socket(int port, char *ip)
{
	struct sockaddr_in		connection;
	struct protoent			*proto;
	struct pollfd			connecting_sock[1];
	const int				yes = 1;
	int						sock;
	int						ok;
	int						size = sizeof(int);

	if ((proto = getprotobyname("tcp")) < 0)
		perror("getprotobyname"), exit(-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		perror("socket"), exit(-1);
	fcntl(sock, F_SETFL, O_NONBLOCK);
	connection.sin_family = AF_INET;
	connection.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &connection.sin_addr) < 0)
		perror("inet_aton"), exit(-1);
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		perror("setsockopt"), exit(-1);
	if (connect(sock, reinterpret_cast< struct sockaddr *>(&connection), sizeof(connection)) < 0)
	{
		if (errno == EINPROGRESS)
		{
			//waiting to connect
			connecting_sock[0].fd = sock;
			connecting_sock[0].events = POLLIN;
			poll(connecting_sock, 1, -1);

			//checking the connection
			getsockopt(sock, SOL_SOCKET, SO_ERROR, &ok, reinterpret_cast< socklen_t * >(&size));
			if (ok)
				std::cout << "async connect failed: " << strerror(ok) << std::endl, exit(-1);
		}
		else
			perror("connect"), exit(-1);
	}
	currentServerSocket = sock;
	return sock;
}

static void setup_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 0;
	term.c_lflag &= static_cast< unsigned int >(~ICANON);
	term.c_lflag &= static_cast< unsigned int >(~ECHO);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &term);
}

static void	sigHandler(int s)
{
	RSA				rsa;
	std::string		sigMessage;

	sigMessage = "\x80" + std::to_string(s);
	rsa.EncodeWrite(currentServerSocket, sigMessage);
}

static void	usage(char *name) __attribute((noreturn));
static void	usage(char *name)
{
	std::cout << "usage: " << name << " < host > [ port ]" << std::endl;
	exit(0);
}

static void reset_terminal(void) __attribute__((destructor));
static void reset_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 1;
	term.c_lflag |= static_cast< unsigned int >(ICANON);
	term.c_lflag |= static_cast< unsigned int >(ECHO);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &term);
}

int			main(int ac, char **av)
{
	int		port = 4242;
	char	*ip = NULL;
	int		server_socket;

	if (ac == 3)
		port = atoi(av[2]);
	if (ac >= 2)
		ip = av[1];
	else
		usage(av[0]);
	if ((server_socket = connect_socket(port, ip)) == -1)
		return (-1);
	signal(SIGINT, sigHandler);
	signal(SIGQUIT, sigHandler);
	setup_terminal();
	client_io(server_socket);
}
