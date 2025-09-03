#include "../../include/User.hpp"

User::User()
{
	;
}

User::User(User const &obj) : ACommand()
{
	(void)obj;
}

User& User::operator=(User const &obj)
{
	(void)obj;
	return (*this);
}

User::~User()
{
	;
}

void User::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
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
	std::cout << " FD = " << (*it)->getUse() << std::endl;
	std::cerr << "COUNT WORD = " << count << std::endl;
	// if (count != 4)//irssi vous en comptez 5
	// {
	// 	output.insert(std::pair<std::string, std::set<int> >(ERR_NEEDMOREPARAMS((*it)->getNick()), fds));
	// 	return ;
	// }

	ss.clear();
	ss.seekg(0);
	std::string username;
	// int mode;
	// char unused;
	std::string realname;

	ss >> username >> realname;
	std::cout << "username: " << username << std::endl;
	// std::cout << "mode: " << "" << mode << std::endl;
	// std::cout << "unused: " << unused << std::endl;
	std::cout << "realname:" << realname << std::endl;

	if (!((*it))->tryRegister())
	{
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	//trop severe sur le format vous refusez irssi
	// if (mode != 0)
	// {
	// 	output.insert(std::pair<std::string, std::set<int> >("USER: arg mode must be zero\n", fds));
	// 	return ;
	// }
	// if (unused != '*')
	// {
	// 	output.insert(std::pair<std::string, std::set<int> >("USER: unused must be 'asterisk'\n", fds));
	// 	return ;
	// }
	// if ((*it)->getUse())
	// {
	// 	output.insert(std::pair<std::string, std::set<int> >(ERR_ALREADYREGISTERED((*it)->getUser()), fds));
	// 	return ;
	// }
	else
	{
		(*it)->setUser(username);
		(*it)->setReal(realname);
		(*it)->setUse(true);
	}

	std::cout << "USERNAME: " << (*it)->getUser() << std::endl;
	std::cout << "REALNAME: " << (*it)->getReal() << std::endl;
}
