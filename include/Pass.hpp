#ifndef PASS_HPP
# define PASS_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Client;

class Server;

class Pass : public ACommand
{
	public:
		Pass();
		Pass(Pass const &obj);
		Pass	&operator=(Pass const &obj);
		virtual ~Pass();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);

};

#endif
