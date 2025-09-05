#include "../../include/Kick.hpp"

Kick::Kick()
{
	;
}

Kick::Kick(Kick const &obj) : ACommand()
{
	(void)obj;
}

Kick& Kick::operator=(Kick const &obj)
{
	(void)obj;
	return (*this);
}

Kick::~Kick()
{
	;
}

void Kick::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;

	std::stringstream ss(args);
	std::string channelsStr;
	std::string usersStr;
	ss >> channelsStr >> usersStr;

	std::string comment;
	std::getline(ss, comment);
	if (!comment.empty() && comment[0] == ' ')
	{
		comment = comment.substr(1);
		comment.erase(comment.size() - 1);

	}
	if (!comment.empty() && comment[0] == ':')
	{
		comment = comment.substr(1);
		comment.erase(comment.size() - 1);
	}
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	if (!(*it)->tryJoinChannel())
	{
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return;
	}

	if (channelsStr.empty() || usersStr.empty())
	{
		output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
		return;
	}

	std::vector<std::string> channels = ACommand::splitAndTrim(channelsStr);
	std::vector<std::string> users = ACommand::splitAndTrim(usersStr);

	// Cas : un seul channel, plusieurs utilisateurs
	if (channels.size() == 1 && users.size() >= 1)
	{
		std::string channelName = channels[0];
		std::map<std::string, Channel*> &serverChannels = server.getChannels();
		std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);

		std::cout << "channelName: " << channelName << std::endl;
		if (ite == serverChannels.end())
		{
			output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
			return;
		}

		Channel *channel = ite->second;

		if (!channel->isOperator((*it)->getNick()))
		{
			output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)].insert((*it)->getFd());
			return;
		}

		for (size_t i = 0; i < users.size(); i++)
		{
			std::string user = users[i];
			std::cout << "		=> USER: " << user << std::endl;
			//Client* target = server.getClientByNick(users[i]);

			// std::vector<Client*> clients = server.getClients();

			// // std::cout << "targetClient name: " << targetClient->getNick() << std::endl;
			// for (size_t i = 0; i < clients.size(); i++)
			// {
			// 	std::cout << "client -> " << i << clients[i]->getNick() << std::endl;
			// 	if (clients[i]->getNick() == user)
			// 	{
			// 		// targetClient = clients[i];
			// 		break;
			// 	}
			// }
			if (!channel->isClientInChannel(user))
			{
				output[ERR_USERNOTINCHANNEL(user, channelName)].insert((*it)->getFd());
				continue;
			}

			// envoyer la notification a tous les membres du channel
			std::cout << "				user: " << user << std::endl;
			std::cout << "				comment: " << comment << std::endl;
			output.insert(std::pair<std::string, std::set<int> >(RPL_KICK((*it)->getNick(), (*it)->getUser(), channelName, user, comment), fds));
			std::set<int> set = channel->noMsgforme((*it));
			output[RPL_KICK((*it)->getNick(), (*it)->getUser(), channelName, user, comment)].insert(set.begin(), set.end());
			channel->removeClientFromChannel(user);
		}
	}
	else // Cas : correspondance 1:1 entre channels[i] et users[i]
	{
		for (size_t i = 0; i < channels.size() && i < users.size(); i++)
		{
			std::string channelName = channels[i];
			std::string user = users[i];
			std::cout << "user: " << user << std::endl;

			//Channel *channel = server.getChannel(channelName);
			std::map<std::string, Channel*> &serverChannels = server.getChannels();
			std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);

			std::cout << "channelName: " << channelName << std::endl;
			if (ite == serverChannels.end())
			{
				output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
				return;
			}

			Channel *channel = ite->second;

			if (!channel->isOperator((*it)->getNick()))
			{
				output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)].insert((*it)->getFd());
				return ;
			}

			std::vector<Client*> clients = server.getClients();
			// Client *targetClient = NULL;

			// for (size_t i = 0; i < clients.size(); i++)
			// {
			// 	if (clients[i]->getNick() == user)
			// 	{
			// 		break;
			// 	}
			// }
			if (!channel->isClientInChannel(user))
			{
				output[ERR_USERNOTINCHANNEL(user, channelName)].insert((*it)->getFd());
				continue;
			}


			std::set<int> set = channel->noMsgforme((*it));
			output[RPL_KICK((*it)->getNick(), (*it)->getUser(), channelName, user, comment)].insert(set.begin(), set.end());
			channel->removeClientFromChannel(user);
		}
	}
}
