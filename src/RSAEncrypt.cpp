#include "RSAEncrypt.hpp"

#define ENCRYPT

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
				reinterpret_cast< unsigned char * >(buff),
				reinterpret_cast< unsigned char * >(decrypted),
				_myKey,
				RSA_PKCS1_OAEP_PADDING) == -1)
	{
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		std::cerr << "Error decrypting message: " << err << std::endl;
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
		s = (size > MSG_BLOCK_SIZE) ? MSG_BLOCK_SIZE : s;
		memcpy(msg_buff, msg, s);

		std::cout << "encrypt length: " << s << "block size: " << MSG_BLOCK_SIZE << std::endl;;
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
		msg += MSG_BLOCK_SIZE;
		s -= s;
		if (s == 0)
			break ;
	}
#else
	write(sock, msg, size);
#endif
	(void)size;
}

size_t		RSAEncrypt::GetMyPublicKey(unsigned char *buff) {
	char	err[0xF00];
	BIO		*bio = BIO_new(BIO_s_mem());
	int		len;

	len = PEM_write_bio_RSA_PUBKEY(bio, _myKey);
	if (!len)
	{
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		std::cerr << "Error encrypting message: " << err << std::endl;
	}
	len = BIO_read(bio, buff, KEY_LENGTH);
	
	BIO_free(bio);
	return static_cast< size_t >(len);
}

void		RSAEncrypt::SetRemotePublicKey(unsigned char * k, size_t size) {
	char			err[0xF00];
	BIO				*bio = BIO_new_mem_buf(k, static_cast< int >(size));
	struct pollfd	fd;

	if (!bio)
		puts("error while allocating space for new public key"), exit(-1);

	if (_remoteKey != NULL)
		RSA_free(_remoteKey);

	_remoteKey = PEM_read_bio_RSA_PUBKEY(bio, &_remoteKey, NULL, NULL);
	if (!_remoteKey)
	{
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		std::cerr << "Error encrypting message: " << err << std::endl;
		puts("error while reading new public key"), exit(-1);
	}

	BIO_free(bio);
}

int		main(void)
{
	RSAEncrypt	RSAEncryptor;
	unsigned char		buff[KEY_LENGTH];
	size_t		size;
	int			fd[2];
	long		r;

	size = RSAEncryptor.GetMyPublicKey(buff);
	RSAEncryptor.SetRemotePublicKey(buff, size);

	pipe(fd);
	char	*str = "crypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\ncrypto !\n";
	RSAEncryptor.WriteTo(fd[1], str, strlen(str));
	std::cout << RSAEncryptor.ReadOn(fd[0], &r);
}
