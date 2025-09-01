#ifndef INVITE_HPP
# define INVITE_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Invite : public ACommand
{
	public:
		Invite();
		Invite(Invite const &obj);
		Invite	&operator=(Invite const &obj);
		virtual ~Invite();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
};

#endif
