#include "RFC.hpp"

RFC::RFC(void) { }
RFC::~RFC(void) { }

void	RFC::DecodeRead(const int sock)
{
	char		buff[0xF00];
	ssize_t		size;

	size = read(sock, buff, 0xF00);
	buff[size] = 0;
	//Analyze(buff, size);
}
/*
void	RFC::Analyze(char *buff, size_t size)
{
	if (buff[0] == '\x81')
		OnPublicKeyReceive(buff, size)
		{
			
		}
	else if (buff[0] == '\x82')
		OnLoginReceive(buff, size)
		{
			
		}
}*/

/*
void	RFC::SendPublicKey(const int sock, unsigned char *key, size_t length)
{
	
}*/

void	RFC::SendLogin(const int sock, const std::string login)
{
	char		buff[130];
	std::size_t	length;
	
	buff[0] = '\x82';
	length = login.copy(buff + 1, 0, login.length());
	buff[length + 1] = 0;
	write(sock, buff, length);
}

void	RFC::SendPassword(const int sock, std::string passwd)
{
	char		buff[120];
	size_t		len;

	buff[0] = '\x83';
	len = passwd.copy(buff + 1, 0, passwd.length());
	buff[len + 1] = 0;
	write(sock, buff, len + 1);
}
/*
void	RFC::SendTerm(const int sock, struct termios term)
{
	char		buff[1024];
	size_t		len;

	buff[0] = '\x84';
	len = sizeof(term);
	len++;
	std::memcpy(buff + 1, &term, sizeof(term));
	buff[len + 1] = 0;
	write(sock, buff, len +1);
}

void	RFC::SendWinsize(const int sock, struct winsize wsize)
{
	char		buff[1024];
	size_t		len;

	buff[0] = '\x85';
	std::memcpy(buff + 1, &wsize, sizeof(wsize));
	len = sizeof(wsize);
	buff[len + 1] = 0;
	write(sock, buff, len + 1);
}
*/
void	RFC::SendText(const int sock, std::string text)
{
	char		buff[2048];
	size_t		len;

	buff[0] = '\x86';
	len = text.copy(buff + 1, 0, text.length());
	buff[len + 1] = 0;
	write(sock, buff, len + 1);
}
