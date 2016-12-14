#include "RSAEncrypt.hpp"
#include <openssl/rsa.h>

char		*RSAEncrypt::_publicKey;
size_t		RSAEncrypt::_publicKeyLength;
char		*RSAEncrypt::_privateKey;
size_t		RSAEncrypt::_privateKeyLength;

void		RSAEncrypt::Init(void)
{
}

void		RSAEncrypt::DeInit(void)
{

}

std::string	RSAEncrypt::ReadOn(const int sock, long *r)
{
	char		buff[0xF000 + 1];

	*r = read(sock, buff, sizeof(buff) - 1);
	if (*r >= 0)
		buff[*r] = 0;
	return std::string(buff);
}

void		RSAEncrypt::WriteTo(const int sock, char *msg, const size_t size)
{
	write(sock, msg, size);
}

const char *	RSAEncrypt::GetPublicKey(void) { return _publicKey; }
void			RSAEncrypt::SetPublicKey(char * k) { _publicKey = k; }
