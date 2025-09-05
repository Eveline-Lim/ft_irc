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
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	if (channelName.empty() || modes.empty())
	{
		output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
		return;
	}
	if (channelName[0] != '#' && channelName[0] != '&')
	{
		if ((*it)->getNick() == channelName)
			return ;
		output[ERR_BADCHANMASK((*it)->getNick(), channelName)].insert((*it)->getFd());
		return ;
	}

	std::map<std::string, Channel*> &channels = server.getChannels();
	std::map<std::string, Channel*>::iterator chanIt = channels.find(channelName);

	if (chanIt == channels.end())
	{
		output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
		return;
	}

	if (!chanIt->second->isOperator((*it)->getNick()))
	{
		output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)].insert((*it)->getFd());
		return;
	}

	char operation = '\0';
	std::queue<std::string> params;

	while (ss >> param)
	{
		std::cout << "			AVANT param: " << param << std::endl;
		if (!param.empty() && param[param.size() - 1] == ',')
		{
			param.erase(param.size() - 1, 1);
		}
		params.push(param);
		std::cout << "		AFTER param: " << param << std::endl;
	}

	bool valid = true;
	std::cout << "			modes: " << modes << std::endl;
	for (std::string::size_type i = 0; i < modes.size(); ++i)
	{
		char c = modes[i];
		if (c == '+' || c == '-')
		{
			operation = c;
			continue ;
		}

		std::string modeFlag;
		modeFlag += operation;
		modeFlag += c;

		std::cout << "modeFlag: " << modeFlag << std::endl;
		if (modeFlag != "i" && modeFlag != "+i" && modeFlag != "-i" && modeFlag != "+k" && modeFlag != "-k" \
			&& modeFlag != "l" && modeFlag != "+l" && modeFlag != "-l" && modeFlag != "o" && modeFlag != "+o" && modeFlag != "-o" \
			&& modeFlag != "t" && modeFlag != "+t" && modeFlag != "-t")
		{
			output[ERR_UNKNOWNMODEFLAG((*it)->getNick())].insert((*it)->getFd());
			valid = false;
			break ;
		}

		if (modeFlag == "i")
		{
			modeFlag = "+i";
		}
		if (modeFlag == "+i" || modeFlag == "-i")
		{
			chanIt->second->modeI((*it)->getNick(), chanIt, modeFlag);
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
					output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
					//valid = false; break ;
					return ;
				}
				password = params.front();
				params.pop();
			}
			chanIt->second->modeK((*it)->getNick(), chanIt, modeFlag, password);
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
					output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
					//valid = false; break ;
					return ;
				}
				limit = atoi(params.front().c_str());
				params.pop();
			}
			chanIt->second->modeL((*it)->getNick(), chanIt, modeFlag, limit);
		}
		if (modeFlag == "o")
		{
			modeFlag = "o";
		}
		if (modeFlag == "+o" || modeFlag == "-o")
		{
			if (params.empty())
			{
				output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
					//valid = false; break ;
				return ;
			}
			std::string target = params.front();
			params.pop();

			chanIt->second->modeO(server, (*it)->getNick(), chanIt, modeFlag, target);
		}
		if (modeFlag == "t")
		{
			modeFlag = "+t";
		}
		if (modeFlag == "+t" || modeFlag == "-t")
		{
			chanIt->second->modeT((*it)->getNick(), chanIt, modeFlag);
		}
	}

	if (valid)
	{
		output[RPL_MODE((*it)->getNick(), (*it)->getUser(), channelName, modes)].insert((*it)->getFd());
		std::set<int> set = chanIt->second->noMsgforme((*it));
		output[RPL_MODE((*it)->getNick(), (*it)->getUser(), channelName, modes)].insert(set.begin(), set.end());
	}
}
