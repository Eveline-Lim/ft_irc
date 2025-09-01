#include "../include/Server.hpp"
#include "../include/Client.hpp"

Client::Client() : _nickname(""), _username(""), _realname(""), _password(""), _ip_add(""), _status(-1), _used(false), _registered(false)
{
	std::cout << "HERRRRE= " << _fd_client << std::endl;
	;
}

Client::Client(int fd) : _fd_client(fd), _nickname(""), _username(""), _realname(""), _password(""), _ip_add(""), _status(-1), _used(false), _registered(false)
{

}

Client::Client(const Client &obj)
{
	this->_fd_client = obj._fd_client;
	this->_nickname = obj._nickname;
	this->_username = obj._username;
	this->_realname = obj._realname;
	this->_password = obj._password;
	this->_ip_add = obj._ip_add;
	this->_status = obj._status;
	this->_used = obj._used;
	this->_registered = obj._registered;
}

Client &Client::operator=(Client const &obj)
{
	this->_fd_client = obj._fd_client;
	this->_nickname = obj._nickname;
	this->_username = obj._username;
	this->_realname = obj._realname;
	this->_password = obj._password;
	this->_ip_add = obj._ip_add;
	this->_status = obj._status;
	this->_used = obj._used;
	this->_registered = obj._registered;
	return (*this);
}

Client::~Client()
{
	;
}

int const &Client::getFd() const
{
	return(this->_fd_client);
}

std::string const &Client::getNick() const
{
	return (this->_nickname);
}

std::string const &Client::getUser() const
{
	return (this->_username);
}

std::string const &Client::getPass() const
{
	return (this->_password);
}

std::string const &Client::getReal() const
{
	return (this->_realname);
}

std::string const &Client::getIp() const
{
	return (this->_ip_add);
}

int &Client::getStatus(void)
{
	return (this->_status);
}

bool Client::getUse()
{
	return (this->_used);
}

bool	Client::getIsNicknameSet(void)
{
	return (_isNicknameSet);
}

void Client::setNick(std::string nickname)
{
	this->_nickname = nickname;
}

void Client::setUser(std::string username)
{
	this->_username = username;
}

void Client::setPass(std::string password)
{
	this->_password = password;
}

void Client::setReal(std::string realname)
{
	this->_realname = realname;
}

void Client::setIp(std::string ipadd)
{
	this->_ip_add = ipadd;
}

void Client::setUse(bool used)
{
	this->_used = used;
}

void Client::setFd(int fd)
{
	this->_fd_client = fd;
}

void Client::setStatus(int status)
{
	this->_status = status;
}

void	Client::setIsNicknameSet(bool isSet)
{
	_isNicknameSet = isSet;
}

bool Client::tryRegister(void)
{
	if (!_password.empty())
	{
		if (!_registered)
		{
			_registered = true;
			std::cout << "Client registered" << std::endl;
		}
	}
	else
	{
		std::cout << "Client must first register with the 'PASS' command" << std::endl;
		return (false);
	}
	return (true);
}

bool Client::isRegistered(void)
{
	return (_registered);
}

bool Client::tryJoinChannel(void)
{
	std::cout << "password: " << _password << std::endl;
	std::cout << "username: " << _username << std::endl;
	std::cout << "nickname: " << _nickname << std::endl;

	if (!_password.empty() && !_nickname.empty() && !_username.empty())
	{
		_registered = true;
		std::cerr << "Client registered" << std::endl;
	}
	else
	{
		std::cerr << "Client must register to access to channels" << std::endl;
		return (false);
	}
	return (true);
}

void Client::setInput(char buffer[1024])
{
	in.insert(in.end(), buffer, buffer + strlen(buffer));
}

bool Client::getInput(std::string &response){
	std::vector<char>::iterator index = std::find(in.begin(), in.end(), '\n');
	if (index == in.end())
		return false;
	response.assign(in.begin(), index);
	std::cerr<<"response : "<<response<<std::endl;
	int space = 1;
	if (*in.begin() == '\r')
		space +=1;
	in.erase(in.begin(), index + space);
	std::cerr<<"reste dasn le vest : "<<std::endl;
	for (size_t i = 0; i < in.size();i++){
		std::cerr<<in[i];
	}
	std::cerr<<"\n"<<std::endl;
	return true;
}
