#include "RSAEncrypt.hpp"

#define ENCRYPT

char *		RSAEncrypt::_publicKey;
size_t		RSAEncrypt::_publicKeyLength;
char *		RSAEncrypt::_privateKey;
size_t		RSAEncrypt::_privateKeyLength;
RSA *		RSAEncrypt::_keypair;

void		RSAEncrypt::Init(void)
{
	RSA			*r = NULL;
	BIGNUM		*e = NULL;
	//BIO			*pb_public = NULL, *pb_private = NULL;
	
	//RSA keypair generation:
	if ((BN_set_word(e, PUB_EXP)) != 1)
		puts("error while initializing RSA\n"), exit(-1);

	r = RSA_new();
	if (RSA_generate_key_ex(_keypair, KEY_LENGTH, e, NULL) != 1)
		puts("failed to generate RSA keypair\n"), exit(-1);

	//to get the 
}

void		RSAEncrypt::DeInit(void)
{

}

std::string	RSAEncrypt::ReadOn(const int sock, long *r)
{
	char		buff[0xF000 + 1];
	char		err[0xF0];
	char		decrypted[KEY_LENGTH * 8];

	*r = read(sock, buff, sizeof(buff) - 1);
	if (*r >= 0)
		buff[*r] = 0;
#ifdef ENCRYPT
	if (RSA_private_decrypt(
				static_cast< int >(*r),
				reinterpret_cast< unsigned char *>(buff),
				reinterpret_cast< unsigned char * >(decrypted),
				_keypair,
				RSA_PKCS1_OAEP_PADDING) == -1)
	{
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		std::cerr << "Error encrypting message: " << err << std::endl;
	}
	return std::string(decrypted);
#else
	return std::string(buff);
#endif
}

void		RSAEncrypt::WriteTo(const int sock, char *msg, const size_t size)
{
	char		err[0xF0];
	char		msg_buff[MSG_BLOCK_SIZE];
	char		encrypted[KEY_LENGTH * 8];
	long		s;
	int			encrypted_length;

#ifdef ENCRYPT
	while (42)
	{
		s = static_cast< long >(strlcpy(msg_buff, msg, MSG_BLOCK_SIZE));

		if ((encrypted_length = RSA_public_encrypt(
						static_cast< int >(s),
						reinterpret_cast< unsigned char *>(msg_buff),
						reinterpret_cast< unsigned char *>(encrypted),
						_keypair,
						RSA_PKCS1_OAEP_PADDING)) == -1)
		{
			ERR_load_crypto_strings();
			ERR_error_string(ERR_get_error(), err);
			std::cerr << "Error encrypting message: " << err << std::endl;
		}
		
		write(sock, encrypted, static_cast< size_t >(encrypted_length));
		if (s == MSG_BLOCK_SIZE)
			break ;
		msg += MSG_BLOCK_SIZE;
	}
#else
	write(sock, msg, size);
#endif
	(void)size;
}

const char *	RSAEncrypt::GetPublicKey(void) { return _publicKey; }
void			RSAEncrypt::SetPublicKey(char * k) { _publicKey = k; }
