#ifndef TOPIC_HPP
# define TOPIC_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Topic : public ACommand
{
	public:
		Topic();
		Topic(Topic const &obj);
		Topic	&operator=(Topic const &obj);
		virtual ~Topic();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
};

#endif
