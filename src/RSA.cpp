#include "RSA.hpp"


RSA::RSA(void)
{
	std::cout << "Default constructor of RSA called" << std::endl;
}

RSA::~RSA(void)
{
	std::cout << "Destructor of RSA called" << std::endl;
}

int		RSA::GetGCD(const int n) const
{
	(void)n;
	return (0);
}

int		RSA::GenerateKeys(const int p, const int q)
{
	(void)p;
	(void)q;
	return (0);
}

std::string	RSA::GetPublicKey(void) const
{
	return _publicKey;
}

std::ostream &	operator<<(std::ostream & o, RSA const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
