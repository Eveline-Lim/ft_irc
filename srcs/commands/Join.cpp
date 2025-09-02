#include "../../include/Join.hpp"

Join::Join()
{
	;
}

Join::Join(Join const &obj) : ACommand()
{
	(void)obj;
}

Join& Join::operator=(Join const &obj)
{
	(void)obj;
	return (*this);
}

Join::~Join()
{
	;
}

void Join::leaveAllChannels(Server &server, Client* client)
{
	std::map<std::string, Channel*> &channels = server.getChannels();
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel *chan = it->second;
		if (chan->isClientInChannel(client->getNick()))
		{
			chan->removeClientFromChannel(client->getNick());
		}
	}
}

void Join::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;

	std::stringstream ss(args);
	std::string channelsStr;
	ss >> channelsStr;
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	std::cout << "channelStr: " << channelsStr << std::endl;
	if (!((*it))->tryJoinChannel())
	{
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	if (channelsStr.empty())
	{
		output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
		return;
	}
	if (channelsStr == "0")
	{
		leaveAllChannels(server, *it);
		// ajouter noms des salons
		std::cout << (*it)->getNick() << "has quit the channels" << std::endl;
		return;
	}

	std::string channelsKeys;
	ss >> channelsKeys;
	std::cout << "channelsKey: " << channelsKeys << std::endl;

	std::vector<std::string> channels = ACommand::splitAndTrim(channelsStr);
	std::vector<std::string> keys = ACommand::splitAndTrim(channelsKeys);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::cout << "channels["<< i <<"]: " << channels[i] << std::endl;
		std::string &channelName = channels[i];

		if (channelName[0] != '#' && channelName[0] != '&')
		{
			output[ERR_BADCHANMASK((*it)->getNick(), channelName)].insert((*it)->getFd());
			return ;
		}

		std::string key = "";
		if (i < keys.size())
		{
			key = keys[i];
			std::cout << "key: " << key << std::endl;
		}

		std::map<std::string, Channel*> &serverChannels = server.getChannels();
		Channel *chan = NULL;
		std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);

		if (ite != serverChannels.end())
		{
			chan = ite->second;
			std::cout << "channelName: " << channelName << std::endl;
			if (!chan->getPassword().empty() && chan->getPassword() != key)
			{
				output[ERR_BADCHANNELKEY((*it)->getNick(), channelName)].insert((*it)->getFd());
				continue ;
			}

			if (chan->isInviteOnly() && !chan->isInvited((*it)->getNick()))
			{
				output[ERR_INVITEONLYCHAN((*it)->getNick(), channelName)].insert((*it)->getFd());
				continue ;
			}

			if (chan->getLimit() != -1 && chan->getNbUsers() >= chan->getLimit())
			{
				output[ERR_CHANNELISFULL((*it)->getNick(), channelName)].insert((*it)->getFd());
				continue ;
			}

			if (!chan->isClientInChannel((*it)->getNick()))
			{
				(*it)->setStatus(MEMBER);
				chan->addClientToChannel(*it);
			}
		}
		else
		{
			std::cout << "channelName: " << channelName << std::endl;
			chan = new Channel(channelName);
			chan->setName(channelName);
			chan->setPassword(key);
			(*it)->setStatus(OPERATOR);
			chan->addClientToChannel((*it));
			server.addChannelToChannels(*chan);
		}
		std::set<int> set = chan->noMsgforme((*it));
		std::cout << "NICK JOIN = " << (*it)->getNick() << std::endl;
		std::cout << "USER JOIN = " << (*it)->getUser() << std::endl;
		std::cout << "CHANNAME JOIN = " << chan->getName() << std::endl;
		std::cout << "CHAN TOPIC = " << chan->getTopic() << std::endl;
		std::string names = chan->getClientsList();
		std::cout << "names: " << names << std::endl;

		output.insert(std::pair<std::string, std::set<int> >(RPL_JOIN((*it)->getNick(), (*it)->getUser(), "JOIN", chan->getName()), fds));
		if (!chan->getTopic().empty())
			output.insert(std::pair<std::string, std::set<int> >(RPL_TOPIC((*it)->getNick(), chan->getName(), chan->getTopic()), fds));
		output.insert(std::pair<std::string, std::set<int> >(RPL_NAMREPLY((*it)->getNick(), chan->getName(), chan->getClientsList()), fds));
		output.insert(std::pair<std::string, std::set<int> >(RPL_ENDOFNAME((*it)->getNick(), chan->getName()), fds));
		output[RPL_JOIN((*it)->getNick(), (*it)->getUser(), "JOIN", chan->getName())].insert(set.begin(), set.end());

		// output[RPL_JOIN((*it)->getNick(), (*it)->getUser(), "JOIN", chan->getName())].insert(set.begin(), set.end());
		// 	output[RPL_TOPIC((*it)->getNick(), chan->getName(), chan->getTopic())].insert(set.begin(), set.end());
		// output[RPL_NAMREPLY((*it)->getNick(), chan->getName(), chan->getClientsList())].insert(set.begin(), set.end());
		//output[RPL_ENDOFNAME((*it)->getNick(),chan->getName())].insert(set.begin(), set.end());
		/* **** DEBUG **** */
		chan->displayMap();
		
		/* **************** */
	}
}
