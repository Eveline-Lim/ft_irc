#ifndef NICK_HPP
# define NICK_HPP

#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"
#include "../include/Server.hpp"

class Client;

class Server;

class Nick : public ACommand
{
	public:
		Nick();
		Nick(Nick const &obj);
		Nick	&operator=(Nick const &obj);
		virtual ~Nick();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
};

#endif
