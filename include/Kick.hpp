#ifndef KICK_HPP
# define KICK_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Kick : public ACommand
{
	public:
		Kick();
		Kick(Kick const &obj);
		Kick	&operator=(Kick const &obj);
		virtual ~Kick();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
};

#endif
