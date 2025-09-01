#ifndef MODE_HPP
# define MODE_HPP

#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/ACommand.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Mode : public ACommand
{
	public:
		Mode();
		Mode(Mode const &obj);
		Mode	&operator=(Mode const &obj);
		virtual ~Mode();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
};

#endif
