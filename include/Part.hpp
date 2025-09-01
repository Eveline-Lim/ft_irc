#ifndef PART_HPP
# define PART_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Part : public ACommand
{
	public:
		Part();
		Part(Part const &obj);
		Part	&operator=(Part const &obj);
		virtual ~Part();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
		std::vector<std::string> splitAndTrim(std::string const &str);

};

#endif
