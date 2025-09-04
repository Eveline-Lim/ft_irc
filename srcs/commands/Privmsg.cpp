#include "../../include/Privmsg.hpp"
#include "../../include/ACommand.hpp"
#include "../../include/Channel.hpp"

Privmsg::Privmsg()
{
	;
}

Privmsg::Privmsg(Privmsg const &obj) : ACommand(obj)
{
	(void)obj;
}

Privmsg& Privmsg::operator=(Privmsg const &obj)
{
	(void)obj;
	return (*this);
}

Privmsg::~Privmsg()
{
	;
}

void	Privmsg::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;
	std::stringstream ss(args);
	std::string word;
	int count = 0;
	std::map<std::string, std::set<int> > &output = server.getOutput();
	std::set<int> fds;
	fds.insert((*it)->getFd());

	while (ss >> word)
	{
		count++;
	}
	if (count < 2)
	{
		output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
		return ;
	}
	ss.clear();
	ss.seekg(0);
	std::string target;
	std::string msg;
	ss >> target;
	getline(ss, msg);
	std::cout << "target: " << target << std::endl;
	std::cout << "		MSG: " << msg << std::endl;
	msg.erase(msg.size() - 1);
	for (size_t i = 0; i < msg.length(); i++)
	{
		std::cout << "msg[i] = " << (int)msg[i] << std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;

	// std::cout << "args: " << args << std::endl;
	// msg = args.substr(target.size() + 1, args.length());
	// std::cout << "msg: | size: " << msg << msg.size() << std::endl;

	// * cannot send to chan
	// ? user away ?
	if (target[0] == '#')
	{
		// target.erase(0, 1);
		// std::cout << "TARGET = " << target << std::endl;
		std::map<std::string, Channel*> &chan = server.getChannels();
		std::map<std::string, Channel*>::iterator jit = chan.find(target);
		if (jit != chan.end())
		{
			std::map<std::string, Client*> clientlist = jit->second->getClientList();
			if (clientlist.find((*it)->getNick()) == clientlist.end())
			{
				std::cout << "ERROR PRVMSG =" << jit->first << std::endl;
				output.insert(std::pair<std::string, std::set<int> >(ERR_NOTONCHANNEL((*it)->getNick(), target), fds));
				return ;
			}
			else
			{
				std::string nick;
				if ((*it)->getStatus() == OPERATOR)
				{
					nick = "@" + (*it)->getNick();
					std::cout << "@nick: " << nick << std::endl;
					std::cout << "fd: " << (*it)->getFd() << std::endl;
				}
				else
				{
					nick = (*it)->getNick();
					// std::cout << "nick: " << nick << std::endl;
					// std::cout << "Sender: nick: " << nick << " | fd: " << (*it)->getFd() << std::endl;
				}
				std::set<int> set = chan[target]->noMsgforme((*it));
				std::cout << "user: " << (*it)->getUser() << std::endl;
				std::cout << "target: " << target << std::endl;
				std::cout << "msg: " << msg << std::endl;
				output[RPL_PRIVMSG(nick, (*it)->getUser(), target, msg)].insert(set.begin(), set.end());
				std::map<std::string, std::set<int> >::iterator msg_it;

				for (msg_it = output.begin(); msg_it != output.end(); msg_it++)
				{
					std::cerr<<"PRIVMSG: "<< msg_it->first<<std::endl;
				}
			}
		}
		else
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHCHANNEL((*it)->getNick(), target), fds));
			return ;
		}
	}
	else
	{
		std::vector<Client*> clients = server.getClients();
		bool	found = false;
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (clients[i]->getNick() == target)
			{
				std::cout << "FOUND !!!" << std::endl;
				std::set<int> fds;
				fds.insert(clients[i]->getFd());
				std::cout << "HEREEEEEEE\n";
				found = true;
				output.insert(std::pair<std::string, std::set<int> >(RPL_PRIVMSG((*it)->getNick(), (*it)->getUser(), target, msg), fds));
				// dire qui a envoye le mesage it->get nickanme
				return ;
			}
		}
		if (found == false)
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHNICK(" ", target), fds));
			return ;
		}

		// std::vector<Client*>::iterator kit;
		// for (kit = client.begin(); kit != client.end(); kit++)
		// {
		// 	std::cout << "PRISG vector: " << (*kit)->getNick() << std::endl;
		// 	std::cout << "target: " << target << std::endl;
		// 	if ((*kit)->getNick() == target)
		// 	{
		// 		std::set<int> fds;
		// 		fds.insert((*kit)->getFd());
		// 		std::cout << "HEREEEEEEE\n";
		// 		output.insert(std::pair<std::string, std::set<int> >(RPL_PRIVMSG((*it)->getNick(), (*it)->getUser(), target, msg), fds));
		// 		// dire qui a envoye le mesage it->get nickanme
		// 		return ;
		// 	}
		// 	else
		// 	{
		// 		output.insert(std::pair<std::string, std::set<int> >(ERR_NOSUCHNICK((*kit)->getNick(), target), fds));
		// 		return ;
		// 	}
		// }

	}
}
