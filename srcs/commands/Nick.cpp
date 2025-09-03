#include "../../include/Nick.hpp"

Nick::Nick()
{
	;
}

Nick::Nick(Nick const &obj) : ACommand()
{
	(void)obj;
}

Nick& Nick::operator=(Nick const &obj)
{
	(void)obj;
	return (*this);
}

Nick::~Nick()
{
	;
}

void Nick::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;
	std::stringstream ss(args);
	std::string word;
	int count = 0;
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	if (!((*it))->tryRegister())
	{
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	while (ss >> word)
	{
		count++;
	}
	if (count < 1)
	{
		output[ERR_NONICKNAMEGIVEN].insert((*it)->getFd());
		return ;
	}
	ss.clear();
	ss.seekg(0);
	std::string nickname;
	ss >> nickname;
	std::string const &newNick = nickname;
	if (newNick.size() > 10)
	{
		return ;
	}
	for (unsigned int i = 0; i < newNick.size(); i++)
	{
		if (newNick[i] == ' ' || newNick[i] == ',' || newNick[i] == '*' || newNick[i] == '?' ||
			newNick[i] == '!' || newNick[i] == '@' || newNick[i] == '.' || newNick[i] == '$' || newNick[i] == ':')
		{
			output[ERR_ERRONEUSNICKNAME((*it)->getNick(), newNick)].insert((*it)->getFd());
			return ;
		}
	}

	std::cout << "newNick: " << newNick << std::endl;
	std::vector<Client*> clients = server.getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getNick() == newNick)
		{
			// std::set<int> fds;
			// fds.insert((*it)->getFd());
			output[ERR_NICKNAMEINUSE((*it)->getNick(), newNick)].insert((*it)->getFd());
			if ((*it)->getIsNicknameSet() == false)
				output[newNick + ":Server :Connection refused, nickname already in use\r\n"].insert((*it)->getFd());
			return;
		}
	}
	std::string oldNick = (*it)->getNick();
	std::cout << "oldNick = " << oldNick << std::endl;
	(*it)->setNick(newNick);
	std::cout << "new nick: " << (*it)->getNick() << std::endl;
	std::cout << "fd: " << (*it)->getFd() << std::endl;
	(*it)->setIsNicknameSet(true);

	std::map<std::string, Channel*> &channels = server.getChannels();
	for (std::map<std::string, Channel*>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt)
	{
		std::map<std::string, Client*> &clientList = chanIt->second->getClientList();
		std::map<std::string, Client*>::iterator userIt = clientList.find(oldNick);

		if (userIt != clientList.end())
		{
			Client *client = userIt->second;
			clientList.erase(userIt);
			clientList[newNick] = client;
		}
	}
	output[RPL_NICK(oldNick, (*it)->getUser(), command, newNick)].insert((*it)->getFd());
	server.FirstThreeCmdsTrue(it);
}
