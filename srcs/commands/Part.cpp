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
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	if (channelsStr.empty())
	{
		output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
		return;
	}
	std::string comment;
	std::getline(ss, comment);
	if (!comment.empty() && comment[0] == ' ')
	{
		comment.erase(0, 1);
	}
	if (!comment.empty() && comment[0] == ':')
	{
		comment.erase(0, 1);
	}
	comment.erase(comment.size() - 1);

	std::cout << "comment: " << comment << std::endl;

	std::vector<std::string> channels = ACommand::splitAndTrim(channelsStr);

	// Boucle sur chaque channel
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string &channelName = channels[i];

		// Verifier que le channel commence par # ou &
		if ((channelName[0] != '#' && channelName[0] != '&'))
		{
			output[ERR_BADCHANMASK((*it)->getNick(), channelName)].insert((*it)->getFd());
			return ;
		}

		// Verifier que le channel existe
		std::map<std::string, Channel*> &serverChannels = server.getChannels();
		std::map<std::string, Channel*>::iterator ite = serverChannels.find(channelName);
		if (ite == serverChannels.end())
		{
			output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
			return ;
		}

		Channel *chan = ite->second;

		// Verifier que le client est dans le channel
		if (!chan->isClientInChannel((*it)->getNick()))
		{
			output[ERR_NOTONCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
			return ;
		}

		chan->removeClientFromChannel((*it)->getNick());
		std::set<int> set = chan->noMsgforme((*it));
		output[RPL_PART((*it)->getNick(), (*it)->getUser(), command, channelName, comment)].insert(set.begin(), set.end());
	}
}
