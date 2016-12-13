#include "RSA.hpp"

RSA::RSA(void)
{
	GenerateKeys("9516311845790656153499716760847001433441357", "65537", "5617843187844953170308463622230283376298685");
}

RSA::~RSA(void)
{
	mpz_clears(_modulo, _publicKey, _privateKey, NULL);
}

int		RSA::GetGCD(int a, int b) const
{
	int		c;

	while (a != 0)
	{
		c = a;
		a = b % a;
		b = c;
	}
	return (b);
}

int		RSA::ModInverse(int a, int m) const
{
	a = a % m;
	for (int x = 1; x < m; x++)
		if ((a * x) % m == 1)
			return x;
	//never reached
	return (-1);
}

int		RSA::GenerateKeys(const char *modulo, const char *publicKey, const char *privateKey)
{
	mpz_init_set_str(_modulo, modulo, 10);
	mpz_init_set_str(_publicKey, publicKey, 10);
	mpz_init_set_str(_privateKey, privateKey, 10);
	return (0);
}

std::string	RSA::DecodeRead(const int sock, long *r) const
{
	char		buff[0xF000 + 1];
/*	long		ret;
	std::string	decoded;
	mpz_t		plainText;
	mpz_t		decodedText;

	mpz_init(plainText);
	mpz_init(decodedText);

	(ret = read(sock, buff, sizeof(buff) - 1));
	*r = ret;
	if (ret > 0)
	{
		buff[ret] = 0;

		mpz_import(decodedText, ret, 1, 1, 0, 0, buff);

		mpz_powm(plainText, decodedText, _privateKey, _modulo);

		mpz_export(buff, NULL, 1, 1, 0, 0, plainText);
		decoded += std::string(buff);
	}
	else
		return "";

	mpz_clears(plainText, decodedText, NULL);

	return decoded;*/
	*r = read(sock, buff, sizeof(buff) - 1);
	if (*r >= 0)
		buff[*r] = 0;
	return std::string(buff);
}

void		RSA::EncodeWrite(const int sock, std::string message) const
{
/*	char		buff[0xF0000];
	size_t		size;
	mpz_t		plainText;
	mpz_t		encodedText;

	mpz_init(plainText);
	mpz_init(encodedText);

	mpz_import(plainText, message.size(), 1, 1, 0, 0, message.c_str());

	//if string is too long
	if (mpz_cmp(plainText, _modulo) > 0)
		return ;

	mpz_powm(encodedText, plainText, _publicKey, _modulo);

	mpz_export(buff, &size, 1, 1, 0, 0, encodedText);
	gmp_printf("Encoded: %Zd\n", encodedText);

	write(sock, buff, size);

	mpz_clears(plainText, encodedText, NULL);*/
	write(sock, message.c_str(), message.size());
}

/*EncodedData	*RSA::Encode(const std::string & message) const
{
	EncodedData	*encoded = new EncodedData();
	mpz_t		plainText;
	mpz_t		encodedText;

	mpz_init(plainText);
	mpz_init(encodedText);

	mpz_import(plainText, message.size(), 1, 1, 0, 0, message.c_str());

	if (mpz_cmp(plainText, _modulo) > 0)
		return "";

	mpz_powm(encodedText, plainText, _publicKey, _modulo);

	size_t	numb = 8;
	size_t	count = mpz_sizeinbase(encodedText, 10);
	if (!(encoded->data = static_cast< char * >(malloc(numb * count))))
		perror("malloc"), raise(SIGKILL);

	mpz_export(encoded->data, &encoded->length, 1, 1, 0, 0, encodedText);

	mpz_clears(plainText, encodedText, NULL);

	return encoded;
}

std::string RSA::Decode(EncodedData *encoded) const
{
	std::string	decoded = "";
	char		*buff;
	mpz_t		plainText;
	mpz_t		decodedText;

	mpz_init(plainText);
	mpz_init(decodedText);

	mpz_import(decodedText, message.size(), 1, 1, 0, 0, message.c_str());

	mpz_powm(plainText, decodedText, _privateKey, _modulo);

	if (!(encoded->data = static_cast< char * >(malloc(8 * mpz_sizeinbase(plainText, 10) + 2))))
		perror("malloc"), raise(SIGKILL);

	mpz_export(buff, NULL, 1, 1, 0, 0, plainText);

	decoded = std::string(buff);
	free(buff);

	mpz_clears(plainText, decodedText, NULL);

	return decoded;
}*/

const char *	RSA::GetPublicKey(void) const { return _publicKeyText; }
void			RSA::SetPublicKey(const char * k)
{
	_publicKeyText = k;
	mpz_init_set_str(_publicKey, k, 10);
}
const char *	RSA::GetModulo(void) const { return _moduloText; }
void			RSA::SetModulo(const char *m)
{
	_moduloText = m;
	mpz_init_set_str(_modulo, m, 10);
}

std::ostream &	operator<<(std::ostream & o, RSA const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
