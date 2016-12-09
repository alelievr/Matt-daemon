#include "RSA.hpp"


RSA::RSA(void)
{
	GenerateKeys(11, 3);
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

int		RSA::GenerateKeys(const int p, const int q)
{
	int		n;
	int		phi;
	int		d;
	float	t;
	int		e = 3;

	n = p * q;
	phi = (p - 1) * (q - 1);
	if (GetGCD(e, p - 1) != 1)
		return -1;
	if (GetGCD(e, q - 1) != 1)
		return -1;
	if (GetGCD(e, phi) != 1)
		return -1;

	d = -1;
	while (42)
	{
		d++;
		t = static_cast< float >(((e * d) - 1)) / static_cast< float >(phi);
		if (fabs(t - static_cast< int >(t)) < std::numeric_limits< float >::epsilon())
			break ;
	}
	std::cout << "d = " << d << std::endl;
	_publicKey = std::to_string(n) + " " + std::to_string(e);
	_privateKey	= std::to_string(n) + " " + std::to_string(d);
	std::cout << _publicKey << " | " << _privateKey << std::endl;

	return (0);
}

std::string	RSA::Encode(const std::string & message)
{
	int			n;
	int			e;
	std::string	encoded("");

	std::stringstream(_publicKey) >> n >> e;

	for (const char & m : message)
		encoded.append(std::to_string(static_cast< char >(static_cast< int >(pow(m, e)) % n)));
	std::cout << "encoded message: " << message;
	return message;
}

std::string RSA::Decode(const std::string & message)
{
	int		d;
	int		n;
	std::string	decoded("");

	for (const char & m : message)
		decoded.append(std::to_string(m));

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
