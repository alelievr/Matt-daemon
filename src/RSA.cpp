#include "RSA.hpp"
#define KEY 0b01010101

RSA::RSA(void)
{
	GenerateKeys(17, 11);
}

RSA::~RSA(void)
{
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

int		RSA::GenerateKeys(const int p, const int q)
{
	int		n;
	int		phi;
	int		d;
	int		e = 3;

	srand(static_cast< unsigned int >(clock()) + static_cast< unsigned int >(time(NULL)));
	n = p * q;
	phi = n - (p + q - 1);

	d = ModInverse(e, phi);
	_publicKey = std::to_string(n) + " " + std::to_string(e);
	_privateKey	= std::to_string(n) + " " + std::to_string(d);

	return (0);
}

std::string		RSA::Encode(const std::string & message) const
{
	int			n;
	int			e;
	std::string	encoded = "";

	std::stringstream(_publicKey) >> n >> e;

	for (const char & m : message)
	{
		//int	c = static_cast< int >(fmod(pow(m, e), n));
		int c = m ^ KEY;
		encoded += std::to_string(c) + " ";
	}
	//std::cout << "encoded string: " << encoded << std::endl;
	return encoded;
}

std::string RSA::Decode(const std::string & message) const
{
	int					d;
	int					n;
	int					m;
	std::string			decoded = "";
	std::stringstream	keys = std::stringstream(message);

	std::stringstream(_privateKey) >> n >> d;

	while (keys >> m)
	{
		//int c = static_cast< int >(fmod(pow(m, d), n));
		int c = m ^ KEY;
		decoded += static_cast< char >(c);
	}
	//std::cout << "decoded messsage: " << decoded;
	(void)d;
	(void)n;
	return decoded;
}

std::string	RSA::GetPublicKey(void) const { return _publicKey; }
void		RSA::SetPublicKey(std::string k) { _publicKey = k; }

std::ostream &	operator<<(std::ostream & o, RSA const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
