#include "RSAEncrypt.hpp"

//#define ENCRYPT

RSAEncrypt::RSAEncrypt() : _remoteKey(NULL)
{
	BIGNUM		*e = NULL;
	
	e = BN_new();
	//RSA keypair generation:
	if ((BN_set_word(e, PUB_EXP)) != 1)
		puts("error while initializing RSA\n"), exit(-1);

	_myKey = RSA_new();
	if (RSA_generate_key_ex(_myKey, KEY_LENGTH, e, NULL) != 1)
		puts("failed to generate RSA keypair\n"), exit(-1);

}

RSAEncrypt::~RSAEncrypt()
{
	RSA_free(_myKey);
}

std::string	RSAEncrypt::ReadOn(const int sock, long *r)
{
	char		buff[0xF000 + 1];

	*r = read(sock, buff, sizeof(buff) - 1);
	if (*r >= 0)
		buff[*r] = 0;

#ifdef ENCRYPT
	char		err[0xF0];
	char		decrypted[KEY_LENGTH * 8];

	if (RSA_private_decrypt(
				static_cast< int >(*r),
				reinterpret_cast< unsigned char *>(buff),
				reinterpret_cast< unsigned char * >(decrypted),
				_myKey,
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
#ifdef ENCRYPT
	char		err[0xF0];
	char		msg_buff[MSG_BLOCK_SIZE];
	char		encrypted[KEY_LENGTH * 8];
	long		s;
	int			encrypted_length;

	while (42)
	{
		s = static_cast< long >(strlcpy(msg_buff, msg, MSG_BLOCK_SIZE));

		if ((encrypted_length = RSA_public_encrypt(
						static_cast< int >(s),
						reinterpret_cast< unsigned char *>(msg_buff),
						reinterpret_cast< unsigned char *>(encrypted),
						_remoteKey,
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

size_t		RSAEncrypt::GetMyPublicKey(unsigned char *buff) {
	int		len = i2d_RSAPublicKey(_myKey, &buff);
	
	return static_cast< size_t >(len);
}

void		RSAEncrypt::SetRemotePublicKey(unsigned char * k, size_t size) {
	BIO	*bio = BIO_new_mem_buf(k, static_cast< int >(size));
	if (!bio)
		puts("error while allocating space for new public key"), exit(-1);

	EVP_PKEY *pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
	if (!pkey)
		puts("error while reading new public key"), exit(-1);

/*	int type = EVP_PKEY_get_type(pkey)
	if (type != EVP_PKEY_RSA2 && type != EVP_PKEY_RSA)
		puts("entered public key is not a RSA key !"), exit(-1);*/

	if (_remoteKey != NULL)
		RSA_free(_remoteKey);

	_remoteKey = EVP_PKEY_get1_RSA(pkey);
	if (_remoteKey == NULL)
		puts("fuck ..."), exit(-1);

	EVP_PKEY_free(pkey);
	BIO_free(bio);
}
