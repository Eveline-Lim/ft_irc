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
	std::string channelsStr, usersStr, comment;
	ss >> channelsStr >> usersStr;
	ss >> comment;

	if (!comment.empty() && comment[0] == ':')
	{
		comment = comment.substr(1);
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
		output[ERR_NEEDMOREPARAMS((*it)->getNick())] = fds;
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
			output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)] = fds;
			return;
		}

		if (!ite->second->isOperator((*it)->getNick()))
		{
			output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)] = fds;
			return;
		}

		for (size_t i = 0; i < users.size(); ++i)
		{
			//Client* target = server.getClientByNick(users[i]);

			std::vector<Client*> clients = server.getClients();
			Client *targetClient = NULL;

			for (size_t i = 0; i < clients.size(); i++)
			{
				std::cout << "client -> " << i << clients[i]->getNick() << std::endl;
				std::cout << "targetClient name: " << targetClient->getNick() << std::endl;
				if (clients[i]->getNick() == targetClient->getNick())
				{
					targetClient = clients[i];
					break;
				}
			}

			if (!targetClient)
			{
				output[ERR_USERNOTINCHANNEL(users[i], channelName)] = fds;
				continue;
			}

			if (!ite->second->isClientInChannel(targetClient->getNick()))
			{
				output[ERR_USERNOTINCHANNEL(targetClient->getNick(), channelName)] = fds;
				continue;
			}

			ite->second->removeClientFromChannel(targetClient->getNick());

			std::string kickMsg = ":" + (*it)->getNick() + " KICK " + channelName + " " + targetClient->getNick();
			if (!comment.empty())
			{
				kickMsg += " :" + comment;
			}
			kickMsg += "\r\n";

			//std::set<int> recipients = channel->getAllClientFds();
			//recipients.insert(targetClient->getFd()); // pour que la cible reçoive le message
			//output[kickMsg] = recipients;
			output[kickMsg];
		}
	}
	else // Cas : correspondance 1:1 entre channels[i] et users[i]
	{
		for (size_t i = 0; i < channels.size() && i < users.size(); i++)
		{
			std::string channelName = channels[i];
			std::string user = users[i];

			//Channel *channel = server.getChannel(channelName);
			std::map<std::string, Channel*> &serverChannels = server.getChannels();
			std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);

			std::cout << "channelName: " << channelName << std::endl;
			if (ite == serverChannels.end())
			{
				output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)] = fds;
				return;
			}

			if (!ite->second->isOperator((*it)->getNick()))
			{
				output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)] = fds;
				continue;
			}

			std::vector<Client*> clients = server.getClients();
			Client *targetClient = NULL;

			for (size_t i = 0; i < clients.size(); i++)
			{
				if (clients[i]->getNick() == targetClient->getNick())
				{
					targetClient = clients[i];
					break;
				}
			}

			if (!targetClient)
			{
				output[ERR_USERNOTINCHANNEL(user, channelName)] = fds;
				continue;
			}

			if (!ite->second->isClientInChannel(user))
			{
				output[ERR_USERNOTINCHANNEL(user, channelName)] = fds;
				continue;
			}

			ite->second->removeClientFromChannel(user);

			std::string kickMsg = ":" + (*it)->getNick() + " KICK " + channelName + " " + user;
			if (!comment.empty())
				kickMsg += " :" + comment;
			kickMsg += "\r\n";

			// std::set<int> recipients = channel->getAllClientFds();
			// recipients.insert(targetClient->getFd());
			// output[kickMsg] = recipients;
			output[kickMsg];
		}
	}
}

// void Kick::execute(Server &server, std::string const &command,
//                    std::vector<Client*>::iterator it, std::string const &args)
// {
//     std::cout << "Entering " << command << " command" << std::endl;

//     std::stringstream ss(args);
//     std::string channelsStr, usersStr, comment;
//     ss >> channelsStr >> usersStr;
//     std::getline(ss, comment);

//     if (!comment.empty() && comment[0] == ' ')
//         comment = comment.substr(1);
//     if (!comment.empty() && comment[0] == ':')
//         comment = comment.substr(1);

//     std::set<int> fds;
//     fds.insert((*it)->getFd());
//     std::map<std::string, std::set<int> > &output = server.getOutput();

//     if (!(*it)->tryJoinChannel())
//     {
//         output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
//         return;
//     }

//     if (channelsStr.empty() || usersStr.empty())
//     {
//         output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
//         return;
//     }

//     std::vector<std::string> channels = splitAndTrim(channelsStr);
//     std::vector<std::string> users = splitAndTrim(usersStr);

//     // Case: one channel, multiple users
//     if (channels.size() == 1 && users.size() >= 1)
//     {
//         std::string channelName = channels[0];
//         std::map<std::string, Channel*> &serverChannels = server.getChannels();
//         std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);

//         if (ite == serverChannels.end())
//         {
//             output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)] = fds;
//             return;
//         }

//         Channel *chan = ite->second;

//         if (!chan->isOperator((*it)->getNick()))
//         {
//             output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)] = fds;
//             return;
//         }

//         for (size_t i = 0; i < users.size(); ++i)
//         {
//             Client *targetClient = NULL;
//             std::vector<Client*> clients = server.getClients();

//             for (size_t j = 0; j < clients.size(); j++)
//             {
//                 if (clients[j]->getNick() == users[i])
//                 {
//                     targetClient = clients[j];
//                     break;
//                 }
//             }

//             if (!targetClient || !chan->isClientInChannel(users[i]))
//             {
//                 output[ERR_USERNOTINCHANNEL(users[i], channelName)] = fds;
//                 continue;
//             }

//             chan->removeClientFromChannel(targetClient->getNick());

//             std::string reason = comment.empty() ? (*it)->getNick() : comment;

//             std::string kickMsg = ":" + (*it)->getNick() + "!" + (*it)->getUser() +
//                                   "@host KICK " + channelName + " " + targetClient->getNick() +
//                                   " :" + reason + "\r\n";

//             std::set<int> recipients = chan->noMsgforme(*it);
//             recipients.insert(targetClient->getFd());
//             output[kickMsg].insert(recipients.begin(), recipients.end());
//         }
//     }
//     else // Case: one-to-one mapping
//     {
//         for (size_t i = 0; i < channels.size() && i < users.size(); i++)
//         {
//             std::string channelName = channels[i];
//             std::string user = users[i];

//             std::map<std::string, Channel*> &serverChannels = server.getChannels();
//             std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);

//             if (ite == serverChannels.end())
//             {
//                 output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)] = fds;
//                 continue;
//             }

//             Channel *chan = ite->second;

//             if (!chan->isOperator((*it)->getNick()))
//             {
//                 output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)] = fds;
//                 continue;
//             }

//             Client *targetClient = NULL;
//             std::vector<Client*> clients = server.getClients();

//             for (size_t j = 0; j < clients.size(); j++)
//             {
//                 if (clients[j]->getNick() == user)
//                 {
//                     targetClient = clients[j];
//                     break;
//                 }
//             }

//             if (!targetClient || !chan->isClientInChannel(user))
//             {
//                 output[ERR_USERNOTINCHANNEL(user, channelName)] = fds;
//                 continue;
//             }

//             chan->removeClientFromChannel(user);

//             std::string reason = comment.empty() ? (*it)->getNick() : comment;

//             std::string kickMsg = ":" + (*it)->getNick() + "!" + (*it)->getUser() +
//                                   "@host KICK " + channelName + " " + user +
//                                   " :" + reason + "\r\n";

//             std::set<int> recipients = chan->noMsgforme(*it);
//             recipients.insert(targetClient->getFd());
//             output[kickMsg].insert(recipients.begin(), recipients.end());
//         }
//     }
// }




// void Kick::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
// {
// 	std::cout << "Entering " << command << " command" << std::endl;
// 	std::stringstream ss(args);
// 	std::string channelsStr;
// 	ss >> channelsStr;
// 	std::set<int> fds;
// 	fds.insert((*it)->getFd());
// 	std::map<std::string, std::set<int> > &output = server.getOutput();

// 	if (!((*it))->tryJoinChannel())
// 	{
// 		output.insert(std::pair<std::string, std::set<int> >("Client must register to access to channels\r\n", fds));
// 		return ;
// 	}
// 	if (channelsStr.empty())
// 	{
// 		output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
// 		return;
// 	}

// 	std::string channelsUsers;
// 	//std::getline(ss, channelsUsers);
// 	ss >> channelsUsers;
// 	std::cout << "channelsUsers: " << channelsUsers << std::endl;

// 	std::vector<std::string> channels = splitAndTrim(channelsStr);
// 	std::vector<std::string> users = splitAndTrim(channelsUsers);

// 	std::string comment;
// 	ss >> comment;

// 	for (size_t i = 0; i < channels.size(); i++)
// 	{
// 		std::cout << "channels["<<i<<"]: " << channels[i] << std::endl;
// 		std::string &channelName = channels[i];

// 		if (channelName[0] != '#' && channelName[0] != '&')
// 		{
// 			std::cerr << "Error: Channel name must start with # or &" << std::endl;
// 			output.insert(std::pair<std::string, std::set<int> >(ERR_BADCHANMASK((*it)->getNick(), channelName), fds));
// 			return ;
// 		}
// 		std::cout << "channelName: " << channelName << std::endl;

// 		std::string user = "";
// 		if (i < users.size())
// 		{
// 			user = users[i];
// 			std::cout << "user: " << user << std::endl;
// 		}

// 		std::map<std::string, Channel*> &serverChannels = server.getChannels();
// 		std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);


// 		if (ite == serverChannels.end())
// 		{
// 			output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHCHANNEL((*it)->getNick(), channelName), fds));
// 			return;
// 		}

// 		// Verifier que le client cible existe dans le serveur
// 		std::vector<Client*> clients = server.getClients();
// 		Client *targetClient = NULL;

// 		for (size_t i = 0; i < clients.size(); ++i)
// 		{
// 			if (clients[i]->getNick() == user)
// 			{
// 				targetClient = clients[i];
// 				break;
// 			}
// 		}

// 		if (!targetClient)
// 		{
// 			output.insert(std::pair<std::string, std::set<int> >(ERR_USERNOTINCHANNEL(user, channelName), fds));
// 			return;
// 		}

// 		// Verification si user a kick est dans le channel
// 		std::cout << "USER: " << user << std::endl;
// 		if (!ite->second->isClientInChannel(user))
// 		{
// 			output.insert(std::pair<std::string, std::set<int> >(ERR_USERNOTINCHANNEL(user, channelName), fds));
// 			return ;
// 		}
// 		// Verifier que l’emetteur est operateur du channel
// 		if (!ite->second->isOperator((*it)->getNick()))
// 		{
// 			output.insert(std::pair<std::string, std::set<int> >(ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName), fds));
// 			return;
// 		}
// 		ite->second->removeClientFromChannel(user);
// 		// if (!ite->second->isClientInInvited((*it)->getNick()))
// 		// {
// 		// 	ite->second->removeClientFromInvited((*it)->getNick());
// 		// }
// 		if (comment.empty())
// 		{
// 			output.insert(std::pair<std::string, std::set<int> >(user + " has been kicked from " + channelName + "\r\n", fds));
// 		}
// 		else
// 		{
// 			output.insert(std::pair<std::string, std::set<int> >(user + " has been kicked from " + channelName + " because " + comment +"\r\n", fds));
// 		}
// 	}
// }
