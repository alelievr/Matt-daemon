/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:30:03 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/11 21:10:04 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ben_afk.hpp"

static void	stdin_event(int server_socket, const RSA & rsa)
{
	char		buff[0xF00];
	long		ret;
	std::string	message;

	if ((ret = read(STDIN_FILENO, buff, sizeof(buff) -1 )) < 0)
		close(server_socket), perror("read"), exit(-1);
	if (ret == 0)
		printf("stdin closed !\n"), exit(0);
	buff[ret] = 0;
	message = std::string(buff);
	rsa.Encode(message);
	write(server_socket, message.c_str(), message.size());
}

static std::string	butify_remote(std::string & message)
{
	int			standardOut = true;
	std::string	ret = "";

	standardOut = static_cast< int >(message[0] - '0');
	ret += (standardOut) ? STDOUT : STDERR;
	message.erase(0, 1);
	for (const char & c : message)
	{
		ret += c;
		if (c == '\n')
			ret += (standardOut) ? STDOUT : STDERR;
	}
	return ret;
}

static void server_event(int server_socket, const RSA & rsa)
{
	char		buff[0xF00];
	long		ret;
	std::string	message;

	std::cout << "\033[D\033[D\033[D   \033[D\033[D\033[D";
	if ((ret = read(server_socket, buff, sizeof(buff) -1 )) < 0)
		close(server_socket), perror("read"), exit(-1);
	if (ret == 0)
		printf("server closed the connection !\n"), exit(0);
	buff[ret] = 0;
	message = std::string(buff);
	rsa.Decode(message);
	message = butify_remote(message);
	std::cout << message;
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
		std::cout << PROMPT << std::flush;
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) < 0)
			close(server_socket), perror("select"), exit(-1);
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
	const int				yes = 1;
	int						sock;

	if ((proto = getprotobyname("tcp")) < 0)
		perror("getprotobyname"), exit(-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		perror("socket"), exit(-1);
	connection.sin_family = AF_INET;
	connection.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &connection.sin_addr) < 0)
		perror("inet_aton"), exit(-1);
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		perror("setsockopt"), exit(-1);
	if (connect(sock, reinterpret_cast< struct sockaddr *>(&connection), sizeof(connection)) < 0)
		perror("bind"), exit(-1);
	return sock;
}

static void	usage(char *name) __attribute((noreturn));
static void	usage(char *name)
{
	std::cout << "usage: " << name << " < host > [ port ]" << std::endl;
	exit(0);
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
	client_io(server_socket);
}
