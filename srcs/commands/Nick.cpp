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

	if (!((*it))->tryRegister())
	{
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	if (nickname.size() > 10)
	{
		output[command + ": max length for nickname is 9\r\n"].insert((*it)->getFd());
		return ;
	}
	for (unsigned int i = 0; i < nickname.size(); i++)
	{
		if (nickname[i] == ' ' || nickname[i] == ',' || nickname[i] == '*' || nickname[i] == '?' ||
			nickname[i] == '!' || nickname[i] == '@' || nickname[i] == '.' || nickname[i] == '$' || nickname[i] == ':')
		{
			output[ERR_ERRONEUSNICKNAME((*it)->getNick(), nickname)].insert((*it)->getFd());
			return ;
		}
	}

	std::cout << "nickname: " << nickname << std::endl;
	std::vector<Client*> clients = server.getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getNick() == nickname && clients[i] != *it)
		{
			// std::set<int> fds;
			// fds.insert((*it)->getFd());
			output[ERR_NICKNAMEINUSE((*it)->getNick(), nickname)].insert((*it)->getFd());
			return;
		}
	}
	// ? boucle pour parcourir vector client et verifier doublons nick client ?
	// if (nickname == (*it)->getNick())
	// {
	// 	output.insert(std::pair<std::string, std::set<int> >(ERR_NICKNAMEINUSE((*it)->getNick(), (*it)->getNick()), fds));
	// 	return ;
	// }
	// if ((*it)->getUse() == false)
	// 	(*it)->setNick(nickname);
	// else
	// {
	// 	(*it)->setNick(nickname);
	// 	output.insert(std::pair<std::string, std::set<int> >(RPL_CHGENICK(nickname), fds));
	// }

	std::string oldNick = (*it)->getNick();
	std::cout << "oldNick = " << oldNick << std::endl;
	(*it)->setNick(nickname);
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
			clientList[nickname] = client;
		}
	}
	output[RPL_NICK(oldNick, (*it)->getUser(), command, (*it)->getNick())].insert((*it)->getFd());
	// (*it)->setNick(nickname);
	std::cout << "new nick123: " << (*it)->getNick() << std::endl;
	// server.FirstThreeCmdsTrue(it);
}
