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

	if (!((*it))->tryJoinChannel())
	{
		output.insert(std::pair<std::string, std::set<int> >("Client must register to access to channels\r\n", fds));
		return ;
	}
	// if (!quitMessage.empty() && quitMessage[0] == ' ')
	// {
	// 	quitMessage.erase(0, 1);
	// }

	// std::cout << "Quit message: " << quitMessage << std::endl;

	// Remove client from all channels
	std::map<std::string, Channel*> &channels = server.getChannels();
	for (std::map<std::string, Channel*>::iterator ite = channels.begin(); ite != channels.end(); ite++)
	{
		if (ite->second->isClientInChannel((*it)->getNick()))
		{
			ite->second->removeClientFromChannel((*it)->getNick());
			output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " removed from " + ite->first + "\n", fds));
		}
	}

	// Envoyer un message a tous les clients du serveur pour leur notifier le depart de x
	// std::string notification = ":" + (*it)->getNick() + " QUIT";
	// if (!quitMessage.empty())
	// {
	// 	notification += " :" + quitMessage;
	// }

	// std::vector<Client*> clients = server.getClients();
	// for (size_t i = 0; i < clients.size(); ++i)
	// {
	// 	if (clients[i] != *it)
	// 	{
	// 		server.sendMsgtoClient(clients[i]->getFd(), notification);
	// 	}
	// }

	// Remove client from server
	server.removeClient((*it)->getNick());
	output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " has quit the server\n", fds));
}
