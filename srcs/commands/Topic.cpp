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
		if (!ite->second->isOperator((*it)->getNick()) && ite->second->isTopicRestricted())
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName), fds));
			return;
		}
		else
		{
			ite->second->setTopic(topicSubject);
			output[RPL_TOPIC((*it)->getNick(), channelName, topicSubject)].insert((*it)->getFd());
			std::set<int> set = ite->second->noMsgforme(*it);
			output[RPL_TOPIC((*it)->getNick(), channelName, topicSubject)].insert(set.begin(), set.end());
		}
	}
}

/*
void Topic::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
    std::cout << "Entering " << command << " command" << std::endl;

    std::stringstream ss(args);
    std::string channelName;
    ss >> channelName;

    std::string topicSubject;
    std::getline(ss, topicSubject);

    if (!topicSubject.empty() && topicSubject[0] == ' ')
        topicSubject.erase(0, 1);
    if (!topicSubject.empty() && topicSubject[0] == ':')
        topicSubject.erase(0, 1);

    std::set<int> fds;
    fds.insert((*it)->getFd());
    std::map<std::string, std::set<int> > &output = server.getOutput();

    // Vérification des paramètres
    if (channelName.empty())
    {
        output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
        return;
    }

    // Vérification si channel existe
    std::map<std::string, Channel*> &channels = server.getChannels();
    std::map<std::string, Channel*>::iterator ite = channels.find(channelName);
    if (ite == channels.end())
    {
        output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
        return;
    }

    Channel *channel = ite->second;

    // Si aucun topic fourni → consultation
    if (topicSubject.empty())
    {
        if (channel->getTopic().empty())
            output[RPL_NOTOPIC((*it)->getNick(), channelName)].insert((*it)->getFd());
        else
            output[RPL_TOPIC((*it)->getNick(), channelName, channel->getTopic())].insert((*it)->getFd());
        return;
    }

    // Sinon → modification du topic
    // Vérifier droits (mode +t = topic protégé)
    if (channel->isTopicProtected() && !channel->isOperator((*it)->getNick()))
    {
        output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)].insert((*it)->getFd());
        return;
    }

    // Appliquer le nouveau topic
    channel->setTopic(topicSubject);

    // Diffuser le changement à tous les clients du channel
    std::string msg = ":" + (*it)->getNick() + "!~" + (*it)->getUser() + "@server TOPIC " + channelName + " :" + topicSubject + "\r\n";
    std::set<int> recipients = channel->getClientFds();
    output[msg].insert(recipients.begin(), recipients.end());
}

*/
