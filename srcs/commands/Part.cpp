#include "../../include/Part.hpp"

Part::Part()
{
	;
}

Part::Part(Part const &obj) : ACommand()
{
	(void)obj;
}

Part& Part::operator=(Part const &obj)
{
	(void)obj;
	return (*this);
}

Part::~Part()
{
	;
}

void Part::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;

	std::stringstream ss(args);
	std::string channelsStr;
	ss >> channelsStr;
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	if (!((*it))->tryJoinChannel())
	{
		output.insert(std::pair<std::string, std::set<int> >("Client must register to access to channels\r\n", fds));
		return ;
	}
	if (channelsStr.empty())
	{
		output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
		return;
	}

	// Recuperer le message optionnel (commentaire)
	std::string comment;
	// std::getline(ss, comment);
	ss >> comment;
	std::cout << "comment: " << comment << std::endl;

	std::vector<std::string> channels = ACommand::splitAndTrim(channelsStr);

	// Boucle sur chaque channel
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string &channelName = channels[i];

		// Verifier que le channel commence par # ou &
		if ((channelName[0] != '#' && channelName[0] != '&'))
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_BADCHANMASK((*it)->getNick(), channelName), fds));
			return ;
		}

		// Verifier que le channel existe
		std::map<std::string, Channel*> &serverChannels = server.getChannels();
		std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);
		if (ite == serverChannels.end())
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHCHANNEL((*it)->getNick(), channelName), fds));
			return ;
		}

		Channel *chan = ite->second;

		// Verifier que le client est dans le channel
		if (!chan->isClientInChannel((*it)->getNick()))
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_NOTONCHANNEL((*it)->getNick(), channelName), fds));
			return ;
		}

		// Supprimer le client du channel
		chan->removeClientFromChannel((*it)->getNick());
		std::cout << (*it)->getNick() << " left " << channelName;
		if (!comment.empty())
		{
			std::cout << " (" << comment << ")" << std::endl;
		}

		// Envoyer message PART au client
		output.insert(std::pair<std::string, std::set<int> >(RPL_PART((*it)->getNick(), (*it)->getReal(), command, channelName, comment), fds));
	}
}
