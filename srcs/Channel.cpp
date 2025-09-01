#include "../include/Channel.hpp"

Channel::Channel() : _name(""), _password(""), _topic(""), _topicRestrictions(false), _isInviteOnly(false), _userLimit(-1), _clients(), _invited()
{
	;
}

Channel::Channel(std::string const &name) : _name(name), _password(""), _topic(""), _topicRestrictions(false), _isInviteOnly(false), _userLimit(-1), _clients(), _invited()
{
	;
}

Channel::Channel(Channel const &obj) : _name(obj._name), _password(obj._password), _topic(obj._topic), _topicRestrictions(obj._topicRestrictions), _isInviteOnly(obj._isInviteOnly), _userLimit(obj._userLimit), _clients(obj._clients), _invited(obj._invited)
{
	;
}

Channel &Channel::operator=(Channel const &obj)
{
	this->_name = obj._name;
	this->_password = obj._password;
	this->_topic = obj._topic;
	this->_topicRestrictions = obj._topicRestrictions;
	this->_isInviteOnly = obj._isInviteOnly;
	this->_userLimit = obj._userLimit;
	this->_clients = obj._clients;
	this->_invited = obj._invited;
	return (*this);
}

Channel::~Channel()
{
	;
}

std::string const &Channel::getName(void) const
{
	return (_name);
}

std::string const &Channel::getPassword(void) const
{
	return (_password);
}

std::string	const &Channel::getTopic(void) const
{
	return (_topic);
}

bool Channel::isTopicRestricted(void) const
{
	return (_topicRestrictions);
}

bool	Channel::isInviteOnly(void) const
{
	return (_isInviteOnly);
}

int	Channel::getLimit(void) const
{
	return (_userLimit);
}

int	Channel::getNbUsers(void) const
{
	return (static_cast<int>(_clients.size()));
}

int	Channel::getStatus(void) const
{
	return (_status);
}

void Channel::setName(std::string &name)
{
	_name = name;
}

void	Channel::setPassword(std::string &password)
{
	_password = password;
}

void Channel::setTopic(std::string const &topic)
{
	_topic = topic;
}

void Channel::setLimit(int &limit)
{
	_userLimit = limit;
}

void Channel::setInvited(bool isInvited)
{
	_isInviteOnly = isInvited;
}

void Channel::setStatus(int status)
{
	_status = status;
}

bool Channel::isInvited(std::string const &nickname)
{
	std::map<std::string, Client*>::iterator it = _clients.find(nickname);

	if (it != _clients.end())
	{
		return (true);
	}
	return (false);
}

bool Channel::isClientInChannel(std::string const &nickname)
{
	std::cout << "nickname: " << nickname << std::endl;
	std::map<std::string, Client*>::iterator it = _clients.find(nickname);

	if (it != _clients.end())
	{
		return (true);
	}
	return (false);
}

bool Channel::isClientInInvited(std::string const &nickname)
{
	std::vector<std::string>::iterator it = find(_invited.begin(), _invited.end(), nickname);

	if (it != _invited.end())
	{
		return (true);
	}
	return (false);
}

void Channel::addClientToChannel(Client* client)
{
	_clients[client->getNick()] = client;
	//std::cout << "client status: " << client->getStatus() << std::endl;
}

void Channel::addInvitedClient(Client *client)
{
	_invited.push_back(client->getNick());
}

void Channel::removeClientFromChannel(std::string const &nickname)
{
	_clients.erase(nickname);
}

void Channel::removeClientFromInvited(std::string const &nickname)
{
	std::vector<std::string>::iterator it = find(_invited.begin(), _invited.end(), nickname);

	_invited.erase(it);
}

std::vector<std::string>& Channel::getInvitedChannels(void)
{
	return (_invited);
}

std::string Channel::getClientsList(void)
{
	std::string clientsList;
	for (std::map<std::string, Client *>::iterator clientsIt = _clients.begin(); clientsIt != _clients.end(); clientsIt++)
	{
		clientsList += clientsIt->second->getNick() + " ";
	}
	return (clientsList);
}

// MODES
bool	Channel::modeI(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg)
{
	std::map<std::string, Client*>::iterator it = _clients.find(client);
	std::cout << "first: " << it->first << std::endl;
	std::cout << "second: " << it->second << std::endl;

	if (it == _clients.end())
	{
		return (false);
	}
	if (it->second->getStatus() != OPERATOR)
	{
		return (false);
	}
	if (arg == "+i")
	{
		ite->second->setInvited(true);
		std::cout << "isInvitedOnly: " << ite->second->isInviteOnly() << std::endl;
		std::cout << "Invite mode set to " << arg << std::endl;
	}
	else if (arg == "-i")
	{
		ite->second->setInvited(false);
		std::cout << "isInvitedOnly: " << ite->second->isInviteOnly() << std::endl;
		std::cout << "Invite mode set to " << arg << std::endl;
	}
	return (true);
}

bool Channel::modeK(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg, std::string &password)
{
	std::map<std::string, Client*>::iterator it = _clients.find(client);

	std::cout << "first: " << it->first << std::endl;
	std::cout << "second: " << it->second << std::endl;

	if (it == _clients.end())
	{
		return (false);
	}
	if (it->second->getStatus() != OPERATOR)
	{
		return (false);
	}
	if (arg == "+k")
	{
		ite->second->setPassword(password);
		std::cout << "_password: " << ite->second->getPassword() << std::endl;
		for (size_t i = 0; i < ite->second->getPassword().size(); i++)
		{
			std::cout << "modeK value = " << (int)password[i] << std::endl;
		}
		std::cout << "Channel key has been set" << std::endl;
	}
	else
	{
		ite->second->_password.erase();
		std::cout << "_password" << ite->second->getPassword() << std::endl;
		std::cout << "Channel key has been removed" << std::endl;
	}
	return (true);
}

bool Channel::modeL(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg, int &limit)
{
	std::map<std::string, Client*>::iterator it = _clients.find(client);

	if (it == _clients.end())
	{
		return (false);
	}
	if (it->second->getStatus() != OPERATOR)
	{
		return (false);
	}
	if (arg == "+l")
	{
		ite->second->setLimit(limit);
		std::cout << "limit: " << ite->second->getLimit() << std::endl;
		std::cout << "Client limited the channel to " << limit << std::endl;
	}
	else
	{
		ite->second->_userLimit = -1;
		std::cout << "limit: " << ite->second->getLimit() << std::endl;
		std::cout << "Client removed the user" << std::endl;
	}
	return (true);
}

bool Channel::modeO(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg, std::string &targetUser)
{
	(void)ite;
	std::map<std::string, Client*>::iterator it = _clients.find(client);
	std::map<std::string, Client*>::iterator iter = _clients.find(targetUser);

	if (it == _clients.end() || (iter == _clients.end()))
	{
		return (false);
	}
	if (it->second->getStatus() != OPERATOR)
	{
		return (false);
	}
	if (arg == "+o")
	{
		iter->second->setStatus(OPERATOR);
		std::cout << "Client granted operator role" << std::endl;
	}
	else
	{
		iter->second->setStatus(MEMBER);
		std::cout << "Client removed channel operator privilege" << std::endl;
	}
	return (true);
}

bool Channel::modeT(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg)
{
	std::map<std::string, Client*>::iterator it = _clients.find(client);
	std::cout << "first: " << it->first << std::endl;
	std::cout << "second: " << it->second << std::endl;

	if (it == _clients.end())
	{
		return (false);
	}
	if (it->second->getStatus() != OPERATOR)
	{
		return (false);
	}
	if (arg == "+t")
	{
		ite->second->_topicRestrictions = true;
		std::cout << "Topic restrictions set to " << arg << std::endl;
	}
	else
	{
		ite->second->_topicRestrictions = false;
		std::cout << "Topic restrictions mode has been removed" << std::endl;
	}
	return (true);
}

void Channel::displayMap(void)
{
	std::cout << "----- DISPLAY MAP -----" << std::endl;
	std::map<std::string, Client*>::iterator	it;

	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "key = " << it->first << " => " << "value = " << it->second->getFd() << std::endl;
	}
	std::cout << std::endl;
}


bool Channel::isOperator(std::string const &nickname)
{
	std::map<std::string, Client*>::const_iterator it = _clients.find(nickname);

	if (it == _clients.end())
	{
		return false;
	}
	return (it->second->getStatus() == OPERATOR);
}

std::set<int> Channel::noMsgforme(Client *client)
{
	std::set<int> set;
	std::map<std::string, Client*> listClient;
	listClient.insert(_clients.begin(), _clients.end());
	for (std::map<std::string, Client*>::iterator it = listClient.begin(); it != listClient.end(); it++)
	{
		if (it->second != client)
			set.insert(it->second->getFd());
		std::cout << "NICK INSIDE NOMSG = " << (it)->second->getNick() << std::endl;
		std::cout << "NICK INSIDE NOMSG FD = " << (it)->second->getFd() << std::endl;
	}
	return (set);
}
// std::string Channel::getClient(std::string const &username) const
// {
// 	std::map<std::string, int>::const_iterator it = _clients.find(username);

// 	if (it != _clients.end())
// 	{
// 		return (it->first);
// 	}
// 	std::cerr << "Error: Client " << username << " not found" << std::endl;
// 	return ("");
// }

// bool Channel::isValid(std::string const &str)
// {
// 	for (size_t i = 1; i < str.length(); i++)
// 	{
// 		if (str[i] <= ' ' || str[i] == ';')
// 		{
// 			return (false);
// 		}
// 	}
// 	return (true);
// }

// void Channel::checkParameterChannel(std::string const &channelName)
// {
// 	if (channelName.empty() == 0)
// 	{
// 		std::cerr << "Error: missing channel name" << std::endl;
// 	}
// 	else if (channelName.length() > 50)
// 	{
// 		std::cerr << "Error: channel name must be with a maximum length of 50" << std::endl;
// 	}
// 	else if (channelName[0] != '#' || channelName[0] != '&')
// 	{
// 		std::cerr << "Error: channel name must begin with a '&' or '#' character" << std::endl;
// 	}
// 	else if (isValid(channelName))
// 	{
// 		std::cerr << "Error: channel name must not contain space or comma" << std::endl;
// 	}
// 	else if (_channels.find(channelName) == _channels.end())
// 	{
// 		std::cout << "Channel " << channelName << " does not exist" << std::endl;
// 	}
// 	else if ((_channels[channelName]->isClientInChannel(client.getNickname())) == false)
// 	{
// 		std::cerr << "Client " << client << " does not exist in channel " << channelName << std::endl;
// 	}
// }

// int Server::getClientFd(std::string const &nickname)
// {
// 	std::vector<Client *>::iterator it;

// 	for (it = idClient.begin(); it != idClient.end(); it++)
// 	{
// 		if ((*it)->getNick() == nickname)
// 		{
// 			return ((*it)->getFd());
// 		}
// 	}
// 	return (-1);
// }
