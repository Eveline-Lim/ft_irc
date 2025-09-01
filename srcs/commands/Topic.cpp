#include "../../include/Topic.hpp"

Topic::Topic()
{
	;
}

Topic::Topic(Topic const &obj) : ACommand()
{
	(void)obj;
}

Topic& Topic::operator=(Topic const &obj)
{
	(void)obj;
	return (*this);
}

Topic::~Topic()
{
	;
}

void Topic::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
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
		output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
		return ;
	}

	ss.clear();
	ss.seekg(0); // ss.str(args);
	std::string channelName;
	std::string topicSubject;
	ss >> channelName >> topicSubject;

	// if (!topicSubject.empty() && topicSubject[0] == ' ')
	// {
	// 	topicSubject.erase(0, 1);
	// }
	std::cout << "channelName: " << channelName << std::endl;
	std::cout << "user: " << topicSubject << std::endl;

	if (!((*it))->tryJoinChannel())
	{
		output.insert(std::pair<std::string, std::set<int> >("Client must register to access to channels\r\n", fds));
		return ;
	}
	if (channelName[0] != '#' && channelName[0] != '&')
	{
		std::cerr << "Error: Channel name must start with # or &" << std::endl;
		output.insert(std::pair<std::string, std::set<int> >(ERR_BADCHANMASK((*it)->getNick(), channelName), fds));
		return ;
	}
	// Verification si channel existe
	std::map<std::string, Channel*> &channels = server.getChannels();
	std::map<std::string, Channel*>::iterator ite = channels.find(channelName);
	if (ite == channels.end())
	{
		output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHCHANNEL((*it)->getNick(), channelName), fds));
		return;
	}
	if (topicSubject.empty())
	{
		output.insert(std::pair<std::string, std::set<int> >("Current topic: " + ite->second->getTopic(), fds));
	}
	else if (topicSubject == " ")
	{
		ite->second->setTopic("");
	}
	else
	{
		// Verifier que l’emetteur est operateur du channel
		if (!ite->second->isOperator((*it)->getNick()))
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName), fds));
			return;
		}
		else
		{
			ite->second->setTopic(topicSubject);
			output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " changed topic to: " + topicSubject, fds));
		}
	}
}
