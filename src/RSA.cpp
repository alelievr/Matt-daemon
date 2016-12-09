#include "RSA.hpp"

RSA::RSA(void)
{
	GenerateKeys(17, 11);
}

RSA::~RSA(void)
{
	std::cout << "Destructor of RSA called" << std::endl;
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
	phi = (p - 1) * (q - 1);

	d = ModInverse(e, phi);
	//std::cout << "d = " << d << ", e = " << e << std::endl;
	_publicKey = std::to_string(n) + " " + std::to_string(e);
	_privateKey	= std::to_string(n) + " " + std::to_string(d);
	//std::cout << _publicKey << " | " << _privateKey << std::endl;

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
	//	std::cout << std::to_string(static_cast< int >(m)) << " pow " << e << " mod " << n;
	//	int	c = static_cast< int >(fmod(pow(m, e), n));
		int c = m ^ 42;
		encoded += std::to_string(c) + " ";
	//	std::cout << " = " << c << std::endl;
	}
	//std::cout << "encoded string: " << encoded << std::endl;
	Decode(encoded);
	return message;
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
	//	std::cout << "ascii: " << std::to_string(m) << " to ";
	//	int c = static_cast< int >(fmod(pow(m, d), n));
		int c = m ^ 42;
	//	std::cout << c << std::endl;
		decoded += static_cast< char >(c);
	}
	//std::cout << "decoded messsage: " << decoded;
	(void)d;
	(void)n;
	return message;
}

std::string	RSA::GetPublicKey(void) const { return _publicKey; }
void		RSA::SetPublicKey(std::string k) { _publicKey = k; }

std::ostream &	operator<<(std::ostream & o, RSA const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
