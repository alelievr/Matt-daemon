#include "RSAEncrypt.hpp"

//#define ENCRYPT

int		RSAEncrypt::_packetID = 0;
int		RSAEncrypt::_pipe[2] = {0, 0};

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

	if (!_pipe[0] && !_pipe[1]) //if pipe is not initialized
		pipe(_pipe);

}

RSAEncrypt::~RSAEncrypt()
{
	RSA_free(_myKey);
}

std::string	RSAEncrypt::ReadOn(const int sock, long *r)
{
#ifdef ENCRYPT
	char		buff[ENCRYPTED_MSG_SIZE];
	char			err[0xF0];
	DataPacket		packet;
	std::string		decrypted = "";
	struct pollfd	reader[1];

	reader[0].fd = sock;
	reader[0].events = POLLIN;
	while (42)
	{
		if ((*r = read(sock, buff, sizeof(buff))) == -1 || *r != ENCRYPTED_MSG_SIZE)
			break ;

		bzero(&packet, sizeof(packet));
		if (RSA_private_decrypt(
					static_cast< int >(*r),
					reinterpret_cast< unsigned char * >(buff),
					reinterpret_cast< unsigned char * >(&packet),
					_myKey,
					RSA_PKCS1_OAEP_PADDING) == -1)
		{
			ERR_load_crypto_strings();
			ERR_error_string(ERR_get_error(), err);
			std::cerr << "Error decrypting message: " << err << std::endl;
		}
		decrypted += packet.data;

		if (poll(reader, 1, 0) == 0) //end of transmitted packets
			break ;
	}
	if (*r != -1)
		*r = static_cast< long >(decrypted.size());
	return decrypted;

#else
	char	buff[0xF000];

	*r = read(sock, buff, sizeof(buff) - 1);
	if (*r >= 0)
		buff[*r] = 0;

	return std::string(buff);

#endif
}

void		RSAEncrypt::WriteTo(const int sock, char *msg, size_t size)
{
#ifdef ENCRYPT
	char		err[0xF0];
	DataPacket	packet;
	char		encrypted[KEY_LENGTH];
	size_t		s;
	int			encrypted_length;

	if (_remoteKey == NULL)
	{
		std::cout << "remote key must be set before sending any datas !" << std::endl;
		return ;
	}

	_packetID++;
	while (42)
	{
		s = (size > MSG_BLOCK_DATA_SIZE) ? MSG_BLOCK_DATA_SIZE : size;
		bzero(&packet, sizeof(packet));
		packet.id = _packetID;
		memcpy(packet.data, msg, s);
		packet.data[s] = 0;
		if ((encrypted_length = RSA_public_encrypt(
						static_cast< int >(sizeof(packet)),
						reinterpret_cast< unsigned char * >(&packet),
						reinterpret_cast< unsigned char * >(encrypted),
						_remoteKey,
						RSA_PKCS1_OAEP_PADDING)) == -1)
		{
			ERR_load_crypto_strings();
			ERR_error_string(ERR_get_error(), err);
			std::cerr << "Error encrypting message: " << err << std::endl;
		}

		write(sock, encrypted, static_cast< size_t >(encrypted_length));
		msg += MSG_BLOCK_DATA_SIZE;
		size -= s;
		if (size <= 0)
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
/*
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
	const char	*str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ2\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\ns";
	RSAEncryptor.WriteTo(fd[1], const_cast< char * >(str), strlen(str) + 1);
	std::cout << RSAEncryptor.ReadOn(fd[0], &r);
}*/
