#include <string>
#include <iostream>

class	RFC
{
	private:
	void	DecodeRead(int sock);
	void	Analyze(std::string message);
	std::function< void(std::string login) >	_SendLogin;
	
	public:
	RFC(void);
	virtual	~RFC()void;

	RFC &	operator=(RFC const & src);

	void	SendLogin(const int sock, const std::string login) const;
	void	SendPassword(std::string passwd);
	void	SendPublicKey(std::string key);
};

std::ostream &	operator<<(std::ostream & o, RFC const & r);
