#include <string>
#include <iostream>
#include <unistd.h>
#include <functional>

class	RFC
{
	private:
	void	DecodeRead(const int sock);
	void	Analyze(char *buff, size_t size);
	void	SendData(const int sock, char *buff, size_t len, char code);
	std::function< void(unsigned char *, size_t) >	_OnPublicKeyReceive;
	std::function< void(std::string login) >		_OnLoginReceive;
	std::function< void(std::string passwd) >		_OnPasswdReceive;
	std::function< void(struct termios term) >		_OnTermReceive;
	std::function< void(struct winsize wsize) >		_OnWinsizeReceive;
	std::function< void(std::string text) >			_OnTextReceive;
	
	public:
	RFC(void);
	virtual	~RFC(void);

	RFC &	operator=(RFC const & src);

	void	SendPublicKey(const int sock, unsigned char *key, size_t size);
	void	SendLogin(const int sock, const std::string login);
	void	SendPassword(const int sock, std::string passwd);
	void	SendTerm(const int sock, struct termios term);
	void	SendWinsize(const int sock, struct winsize wsize);
	void	SendText(const int sock, std::string text);
};

std::ostream &	operator<<(std::ostream & o, RFC const & r);
