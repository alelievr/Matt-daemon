#include "RFC.hpp"

RFC::RFC(void) { }
RFC::~RFC(void) { }

void	RFC::DecodeRead(const int sock)
{
	char	buff[0xF00];
	int		size;

	size = read(sock, buff, 0xF00);
	buff[size] = 0;

}

void	RFC::Analyze(char *buff, int size)
{
	if (buff[0] == '\x81')
		OnPublicKeyReceive(buff, size)
		{
			
		}
	else if (buff[0] == '\x82')
		OnLoginReceive(buff, size)
		{
			
		}
}

void	RFC::SendPublicKey(const int sock, std::string Key)

void	RFC::SendLogin(const int sock, const std::string Login)
{
	char		buff[130];
	std::size_t	length;
	
	buff[0] = '\x82';
	length = Login.copy(buff + 1, 0, Login.length);
	buff[length + 1] = 0;
	write(sock, buff, length);
}
