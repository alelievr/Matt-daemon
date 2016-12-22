#include <string>
#include <iostream>

class	RFC
{
	private:
	void	DecodeRead(const int sock);
	void	Analyze(char *buff, int size);
	std::function< void(std::string PublicKey) >	_OnPublicKeyReceive;
	std::function< void(std::string login) >		_OnLoginReceive;
	std::function< void(std::string passwd) >		_OnPasswdReceive;
	std::function< void(struct termios term) >		_OnTermReceive;
	std::function< void(struct winsize wsize) >		_OnWinsizeReceive;
	std::function< void(std::string text) >			_OnTextReceive;
	
	public:
	RFC(void);
	virtual	~RFC()void;

	RFC &	operator=(RFC const & src);

	void	SendPublicKey(const int sock, std::string Key);
	void	SendLogin(const int sock, const std::string Login);
	void	SendPassword(const int sock, std::string Passwd);
	void	SendTerm(const int sock, struct termios Term);
	void	SendWinsize(const int sock, struct winsize Wsize);
	void	SendText(const int sock, std::string Text);
};

std::ostream &	operator<<(std::ostream & o, RFC const & r);
