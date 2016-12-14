#include "RFC.hpp"

RFC::RFC(void) { }
RFC::~RFC(void) { }

void	RFC::Analyze(std::string message)
{
	if (message[0] == '\x81')
		SendLogin(message.erase(0, 1)) {  }
}

/*void	RFC::SendLogin(const int sock, const std::string Login)
{
	RSA	rsa;

	rsa.EncodeWrite(sock, "\x81" + login);
}*/
