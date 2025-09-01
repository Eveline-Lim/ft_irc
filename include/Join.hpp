#ifndef JOIN_HPP
# define JOIN_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Join : public ACommand
{
	public:
		Join();
		Join(Join const &obj);
		Join	&operator=(Join const &obj);
		virtual ~Join();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
		void leaveAllChannels(Server &server, Client* client);
};

#endif
