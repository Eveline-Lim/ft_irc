#ifndef USER_HPP
# define USER_HPP

#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/ACommand.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class User : public ACommand
{
	public:
		User();
		User(User const &obj);
		User	&operator=(User const &obj);
		virtual ~User();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
};

#endif
