#include "../../include/Mode.hpp"

Mode::Mode()
{
	;
}

Mode::Mode(Mode const &obj) : ACommand()
{
	(void)obj;
}

Mode& Mode::operator=(Mode const &obj)
{
	(void)obj;
	return (*this);
}

Mode::~Mode()
{
	;
}

void Mode::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;
	std::stringstream ss(args);
	std::string channelName;
	std::string modes;
	std::string param;

	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	ss >> channelName >> modes;
	std::cout << "channelName: " << channelName << std::endl;
	std::cout << "modes: " << modes << std::endl;

	if (!((*it))->tryJoinChannel())
	{
		output.insert(std::pair<std::string, std::set<int> >("Client must register to access to channels\r\n", fds));
		return ;
	}
	if (channelName.empty() || modes.empty())
	{
		output.insert(std::make_pair(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
		return;
	}
	if (channelName[0] != '#' && channelName[0] != '&')
	{
		if ((*it)->getNick() == channelName)
			return ;
		std::cerr << "Error: Channel name must start with # or &" << std::endl;
		output.insert(std::pair<std::string, std::set<int> >(ERR_BADCHANMASK((*it)->getNick(), channelName), fds));
		return ;
	}

	std::map<std::string, Channel*> &channels = server.getChannels();
	std::map<std::string, Channel*>::iterator chanIt = channels.find(channelName);

	if (chanIt == channels.end())
	{
		output.insert(std::make_pair(ERR_NOSUCHCHANNEL((*it)->getNick(), channelName), fds));
		return;
	}

	if (!chanIt->second->isOperator((*it)->getNick()))
	{
		output.insert(std::make_pair(ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName), fds));
		return;
	}

	char operation = '\0';
	std::queue<std::string> params;

	while (ss >> param)
	{
		params.push(param);
		std::cout << "param: " << param << std::endl;
	}

	for (std::string::size_type i = 0; i < modes.size(); ++i)
	{
		char c = modes[i];
		if (c == '+' || c == '-')
		{
			operation = c;
			continue;
		}

		std::string modeFlag;
		modeFlag += operation;
		modeFlag += c;

		std::cout << "modeFlag: " << modeFlag << std::endl;
		if (modeFlag != "i" && modeFlag != "+i" && modeFlag != "-i" && modeFlag != "+k" && modeFlag != "-k" \
			&& modeFlag != "l" && modeFlag != "+l" && modeFlag != "-l" && modeFlag != "o" && modeFlag != "+o" && modeFlag != "-o" \
			&& modeFlag != "t" && modeFlag != "+t" && modeFlag != "-t")
		{
			output.insert(std::make_pair("Unknown mode flag: " + modeFlag + "\n", fds));
		}

		if (modeFlag == "i")
		{
			modeFlag = "+i";
		}
		if (modeFlag == "+i" || modeFlag == "-i")
		{
			chanIt->second->modeI((*it)->getNick(), chanIt, modeFlag);
			// if (chanIt->second->modeI((*it)->getNick(), chanIt, modeFlag))
			// {
			// 	std::cout << "Invite mode set to" <<  chanIt->second->isInviteOnly() << std::endl;
			// 	output.insert(std::pair<std::string, std::set<int> >("Invite mode set to " + chanIt->second->isInviteOnly(), fds));
			// }
			// else
			// {
			// 	output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHCHANNEL((*it)->getNick(), channelName), fds));
			// }
		}

		if (modeFlag == "k")
		{
			modeFlag = "+k";
		}
		if (modeFlag == "+k" || modeFlag == "-k")
		{
			std::string password = "";
			if (modeFlag == "+k")
			{
				if (params.empty())
				{
					output.insert(std::make_pair(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
					return ;
				}
				password = params.front();
				params.pop();
			}
			chanIt->second->modeK((*it)->getNick(), chanIt, modeFlag, password);
			// if (chanIt->second->modeK((*it)->getNick(), chanIt, modeFlag, password))
			// {
			// 	std::cout << "new password: " << password << std::endl;
			// 	output.insert(std::pair<std::string, std::set<int> >( "Channel key has been changed\n", fds));
			// }
			// else
			// {
			// 	output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " cannot change channel key\n", fds));
			// }
		}
		if (modeFlag == "l")
		{
			modeFlag = "+l";
		}
		if (modeFlag == "+l" || modeFlag == "-l")
		{
			int limit = 0;
			if (modeFlag == "+l")
			{
				if (params.empty())
				{
					output.insert(std::make_pair(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
					return ;
				}
				limit = atoi(params.front().c_str());
				params.pop();
			}
			chanIt->second->modeL((*it)->getNick(), chanIt, modeFlag, limit);
			// if (chanIt->second->modeL((*it)->getNick(), chanIt, modeFlag, limit))
			// {
			// 	std::cout << "User " << (*it)->getNick() << " limited the channel to " << chanIt->second->getLimit() << " user(s)\n" << std::endl;
			// 	// output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " limited the channel to " + chanIt->second->getLimit() + " user(s)\n", fds));
			// }
			// else
			// {
			// 	output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " cannot change user limit", fds));
			// }
		}
		if (modeFlag == "o")
		{
			modeFlag = "o";
		}
		if (modeFlag == "+o" || modeFlag == "-o")
		{
			if (params.empty())
			{
				output.insert(std::make_pair(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
				return ;
			}
			std::string target = params.front();
			params.pop();

			chanIt->second->modeO((*it)->getNick(), chanIt, modeFlag, target);
			// if (chanIt->second->modeO((*it)->getNick(), chanIt, modeFlag, target))
			// {
			// 	output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " granted " + target + "the operator role\n", fds));
			// }
			// else
			// {
			// 	output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " cannot change channel operator privilege\n", fds));
			// }
		}
		if (modeFlag == "t")
		{
			modeFlag = "+t";
		}
		if (modeFlag == "+t" || modeFlag == "-t")
		{
			chanIt->second->modeT((*it)->getNick(), chanIt, modeFlag);
			// if (chanIt->second->modeT((*it)->getNick(), chanIt, modeFlag))
			// {
			// 	std::cout << "User " << (*it)->getNick() << " changed topic restrictions to " << chanIt->second->isTopicRestricted() << std::endl;
			// }
			// else
			// {
			// 	output.insert(std::pair<std::string, std::set<int> >((*it)->getNick() + " cannot change topic restriction\n", fds));
			// }
		}
		// output[RPL_MODE((*it)->getNick(), (*it)->getUser(), ...)].insert((*it)->getFd());
	}
}
