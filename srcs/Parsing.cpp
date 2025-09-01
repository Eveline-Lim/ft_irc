#include "../include/Server.hpp"

bool	Server::isValidPort(std::string const &str)
{
	if (str.length() != 4)
	{
		return (false);
	}
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
		{
			return (false);
		}
	}
	this->_port = atoi(str.c_str());
	if (this->_port < 6665|| this->_port > 6669)
	{
		return (false);
	}
	return (true);
}

bool	Server::isValidPassword(std::string const &str)
{
	if (str.length() > 10)
	{
		return (false);
	}
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isalnum(str[i]))
		{
			return (false);
		}
	}
	this->_password = str;
	return (true);
}

void	Server::checkArgs(char *argv[])
{
	std::string	port;
	std::string	password;

	port = argv[1];
	password = argv[2];
	if (port.empty() || password.empty())
	{
		throw std::invalid_argument("Empty input");
	}
	if (!isValidPort(port))
	{
		throw std::invalid_argument("Only ports between 6665 and 6669 are used");
	}
	if (!isValidPassword(password))
	{
		throw std::invalid_argument("Password must only contain alphanumeric characters with a maximum length of 10 characters");
	}
}
