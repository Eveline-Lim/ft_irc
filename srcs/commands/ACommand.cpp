#include "../../include/ACommand.hpp"

// std::set<int> ACommand::retrieveClient(Server &server, Channel &channel, std::string nickname)
// {
// 	std::cout << "CHANNEL WHAT INSIDE = " << &channel << std::endl;
// 	std::map<std::string, int> clients = channel.getClientList();
// 	std::set<int> fds;
// 	std::map<std::string, int>::iterator it;

// 	for (it = clients.begin(); it != clients.end(); it++)
// 	{
// 		int fd = server.getClientFd(nickname);
// 		if (fd != -1)
// 		{
// 			fds.insert(fd);
// 		}
// 	}
// 	return (fds);
// }

std::vector<std::string> ACommand::splitAndTrim(std::string const &str)
{
	std::vector<std::string> result;
	size_t start = 0;

	std::cout << "str = " << str << std::endl;
	while (true)
	{
		size_t pos = str.find(',', start);
		std::string token;

		if (pos == std::string::npos)
		{
			token = str.substr(start);
			std::cout << "token if: " << token << std::endl;
		}
		else
		{
			token = str.substr(start, pos - start);
			std::cout << "token else: " << token << std::endl;
		}
		// size_t first = token.find_first_not_of(" \t");
		// size_t last = token.find_last_not_of(" \t");
		// if (first != std::string::npos)
		// {
		// 	token = token.substr(first, last - first + 1);
		// }
		// else
		// {
		// 	token.clear();
		// }
		if (!token.empty())
		{
			std::cout << "token avant ajout " << token << std::endl;
			result.push_back(token);
		}
		if (pos == std::string::npos)
		{
			break;
		}
		start = pos + 1;
	}
	return (result);
}
