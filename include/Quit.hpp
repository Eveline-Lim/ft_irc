#ifndef QUIT_HPP
# define QUIT_HPP

#include "../include/Server.hpp"
#include "../include/ACommand.hpp"
#include "../include/Client.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Server;

class Client;

class Quit : public ACommand
{
	public:
		Quit();
		Quit(Quit const &obj);
		Quit	&operator=(Quit const &obj);
		virtual ~Quit();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);

};

#endif
