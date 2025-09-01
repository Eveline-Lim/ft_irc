#ifndef Privmsg_HPP
#define Privmsg_HPP

#include "../include/ACommand.hpp"
#include "../include/Channel.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"

class Channel;

class Privmsg : public ACommand
{
	public:
		Privmsg();
		Privmsg(Privmsg const &obj);
		Privmsg	&operator=(Privmsg const &obj);
		virtual ~Privmsg();

		virtual void execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args);
		std::set<std::string>	noMsgforme(Channel &channel, Client *client);
	};

#endif
