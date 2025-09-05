#include "../../include/Quit.hpp"

Quit::Quit()
{
	;
}

Quit::Quit(Quit const &obj) : ACommand()
{
	(void)obj;
}

Quit& Quit::operator=(Quit const &obj)
{
	(void)obj;
	return (*this);
}

Quit::~Quit()
{
	;
}

void Quit::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;

	std::string quitMessage;
	std::stringstream ss(args);
	std::getline(ss, quitMessage);

	//ss >> quitMessage;
	if (!quitMessage.empty() && quitMessage[0] == ' ')
	{
		quitMessage.erase(0, 1);
		quitMessage.erase(quitMessage.size() - 1);
	}
	if (!quitMessage.empty() && quitMessage[0] == ':')
	{
		quitMessage.erase(0, 1);
		quitMessage.erase(quitMessage.size() - 1);
	}
	std::cout << "Quit message: " << quitMessage << std::endl;

	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	// Remove client from all channels
	std::map<std::string, Channel*> &channels = server.getChannels();
	for (std::map<std::string, Channel*>::iterator ite = channels.begin(); ite != channels.end(); )
	{
		Channel *chan = ite->second;
		if (chan->isClientInChannel((*it)->getNick()))
		{
			std::string msg;
			std::set<int> set = chan->noMsgforme((*it));
			if (!quitMessage.empty())
			{
				msg = ":" + (*it)->getNick() + "!~" + (*it)->getUser() + "@server QUIT :Quit " + quitMessage + "\r\n";
			}
			else
			{
				msg = ":" + (*it)->getNick() + "!~" + (*it)->getUser() + "@server QUIT :Quit \r\n";
			}
			output[msg].insert(set.begin(), set.end());
			chan->removeClientFromChannel((*it)->getNick());
			if (chan->getClientList().size() == 0)
				server.removeChannel(chan->getName());
			else
				++ite;
		}
		else
		{
			++ite;
		}
	}
	server.closeAllfd((*it)->getFd());
	// server.removeClient((*it)->getNick());

	// close((*it)->getFd());
	// (*it)->setSuppressed(true);
	// std::vector<Client*> clients = server.getClients();
	// for (size_t i = 0; i < clients.size(); ++i)
	// {
	// 	if (clients[i] != *it)
	// 	{
	// 		server.sendMsgtoClient(clients[i]->getFd(), notification);
	// 	}
	// }

	// Remove client from server

	// server.closeFd();
	// server.closeAllfd((*it)->getFd());
	// server.freeClients();
}
