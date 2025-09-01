#include "../../include/Pass.hpp"

Pass::Pass()
{
	;
}

Pass::Pass(Pass const &obj) : ACommand()
{
	(void)obj;
}

Pass& Pass::operator=(Pass const &obj)
{
	(void)obj;
	return (*this);
}

Pass::~Pass()
{
	;
}

void Pass::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;
	std::cout << "args: " << args << std::endl;
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
	std::cout << "count = " << count << std::endl;
	if (count != 1)
	{
		output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
		return ;
	}

	ss.clear();
	ss.seekg(0);
	std::string password;
	ss >> password;

	if (password != server.getPassword())
	{
		output.insert(std::pair<std::string, std::set<int> >(ERR_PASSWDMISMATCH((*it)->getNick()), fds));
		return ;
	}
	if ((*it)->getUse() == true)
	{
		output.insert(std::pair<std::string, std::set<int> >(ERR_ALREADYREGISTERED((*it)->getNick()), fds));
		return ;
	}
	else
		(*it)->setPass(password);

	std::cerr << "PASSWORD: " << (*it)->getPass() << std::endl;
}

// void Pass::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, const std::string &args)
// {
// 	(void)server;
// 	(void)args;
// 	(void)it;
// 	std::cout << "Entering " << command << " command" << std::endl;
// 	// std::stringstream ss(args);
// 	// std::string word;
// 	// int count = 0;
// 	// std::set<int> fds;
// 	// fds.insert((*it)->getFd());
// 	// std::map<std::string, std::set<int> > &output = server.getOutput();

// 	// while (ss >> word)
// 	// {
// 	// 	count++;
// 	// }
// 	// std::cout << "count = " << count << std::endl;
// 	// if (count != 1)
// 	// {
// 	// 	output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
// 	// 	return ;
// 	// }

// 	// ss.clear();
// 	// ss.seekg(0);
// 	// std::string password;
// 	// ss >> password;
// 	// if ((*it)->getUse() == true)
// 	// {
// 	// 	output.insert(std::pair<std::string, std::set<int> >(ERR_ALREADYREGISTERED((*it)->getNick()), fds));
// 	// 	return ;
// 	// }
// 	// else
// 	// 	(*it)->setPass(password);

// 	// std::cout << "PASSWORD: " << (*it)->getPass() << std::endl;
// }
