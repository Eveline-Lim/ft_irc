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
	//std::getline(ss, quitMessage);
	ss >> quitMessage;
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	// if (!quitMessage.empty() && quitMessage[0] == ' ')
	// {
	// 	quitMessage.erase(0, 1);
	// }

	// std::cout << "Quit message: " << quitMessage << std::endl;

	// Remove client from all channels
	std::map<std::string, Channel*> &channels = server.getChannels();
	for (std::map<std::string, Channel*>::iterator ite = channels.begin(); ite != channels.end(); )
	{
		if (ite->second->isClientInChannel((*it)->getNick()))
		{
			std::string msg;
			std::set<int> set = ite->second->noMsgforme((*it));
			if (!quitMessage.empty())
			{
				msg = ":" + (*it)->getNick() + "!~" + (*it)->getUser() + "@server QUIT :Quit " + quitMessage + "\r\n";
			}
			else
			{
				msg = ":" + (*it)->getNick() + "!~" + (*it)->getUser() + "@server QUIT :Quit \r\n";
			}
			output[msg].insert(set.begin(), set.end());
			ite->second->removeClientFromChannel((*it)->getNick());
			std::map<std::string, Channel*>::iterator tmpIt = ite;
			std::cout << "tmpIt:" <<  tmpIt->first << std::endl;
			++tmpIt;
			if (ite->second->getClientList().size() == 0)
			{
				server.removeChannel(ite->second->getName());
			}
			ite = tmpIt;
		}
		else
		{
			++ite;
		}
	}

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
	// close((*it)->getFd());
	// server.removeClient((*it)->getNick());

}
