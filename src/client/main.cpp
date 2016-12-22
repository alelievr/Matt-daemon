/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:30:03 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/14 15:08:37 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ben_afk.hpp"

static int			currentServerSocket;
static RSAEncrypt	*RSAEncryptor;

static void	stdin_event(int server_socket)
{
	char		buff[0xF00];
	long		ret;

	if ((ret = read(STDIN_FILENO, buff, sizeof(buff) -1 )) < 0)
		close(server_socket), exit(-1);
	if (ret == 0 || buff[0] == '\x04')
		printf("quitting client ...\n"), exit(0);
	buff[ret] = 0;
	RSAEncryptor->WriteTo(server_socket, buff, static_cast< size_t >(ret));
}

static void server_event(int server_socket)
{
	long		ret;
	std::string	message;

	if ((message = RSAEncryptor->ReadOn(server_socket, &ret)).empty())
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
					stdin_event(server_socket);
				else if (i == server_socket)
					server_event(server_socket);
				else
					close(i);
			}
	}
}

static int	connect_socket(int port, char *ip)
{
	struct sockaddr_in		connection;
	struct protoent			*proto;
	int						sock;

	if ((proto = getprotobyname("tcp")) < 0)
		perror("getprotobyname"), exit(-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		perror("socket"), exit(-1);
	endprotoent();
	connection.sin_family = AF_INET;
	connection.sin_port = htons(static_cast< uint16_t >(port));
	if (inet_pton(AF_INET, ip, &connection.sin_addr) < 0)
		perror("inet_aton"), exit(-1);
	if (connect(sock, reinterpret_cast< struct sockaddr *>(&connection), sizeof(connection)) < 0)
		perror("connect"), exit(-1);
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

static int	resolve_host(char *ip, char *host, const char *port)
{
	struct addrinfo	*info;
	struct addrinfo	hints;
	void			*ptr;
	int				s;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	if ((s = getaddrinfo(host, port, &hints, &info)) != 0)
		printf("getaddrinfo: %s\n", gai_strerror(s));
	while (info)
	{
		ptr = &(reinterpret_cast< struct sockaddr_in * >(info->ai_addr))->sin_addr;
		if (!(inet_ntop(info->ai_family, ptr, ip, INET_ADDRSTRLEN)))
			return -1;
		info = info->ai_next;
	}
	return (0);
}

static void	sigHandler(int s)
{
	std::string		sigMessage;

	sigMessage = "\x80" + std::to_string(s);
	RSAEncryptor->WriteTo(currentServerSocket, const_cast< char *>(sigMessage.c_str()), sigMessage.size());
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
	delete RSAEncryptor;
}

int			main(int ac, char **av)
{
	const char	*portstr = "4242";
	char	*host = NULL;
	int		port;
	char	ip[INET_ADDRSTRLEN];
	int		server_socket;

	if (ac == 3)
		portstr = av[2];
	if (ac >= 2)
		host = av[1];
	else
		usage(av[0]);
	port = atoi(portstr);
	if (resolve_host(ip, host, portstr) == -1)
		perror("can't resolve hostname/ip"), exit(-1);
	if ((server_socket = connect_socket(port, ip)) == -1)
		return (-1);
	signal(SIGINT, sigHandler);
	signal(SIGQUIT, sigHandler);
	setup_terminal();
	RSAEncryptor = new RSAEncrypt();
	client_io(server_socket);
}
