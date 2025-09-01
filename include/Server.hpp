#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "Errors.hpp"
#include "Replies.hpp"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h> // * acceder a struct hostent (infos machines hote)
#include <netinet/in.h> // * library for ipv6
#include <poll.h>
#include <sstream>
#include <strings.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <ctime>

extern bool	signalGlobal;

enum e_userCommands {
	PASS,
	USER,
	NICK,
	PRIVMSG,
	CAP,
	JOIN,
	KICK,
	INVITE,
	MODE,
	TOPIC,
	PART,
	QUIT,
	PING
};

class Client;

class Channel;

class Server
{
	public:
		Server();
		Server(Server const &obj);
		Server	&operator=(Server const &obj);
		~Server();

		std::string const &getPassword(void) const;

		void	setInvited(std::string const &client);
		void	setTopic(std::string const &topic);

		void	checkArgs(char *argv[]);
		bool	isValidPort(std::string const &str);
		bool	isValidPassword(std::string const &str);
		static void	sigintHandler(int signal);
		void	setSignal(void);

		void	initServer(Server &server);
		bool	checkPoll(Server &server);

		void	newClient();
		bool	newData(int);
		void	addClients();
		void	deleteClients(int i);
		bool	executeCommands(char *buffer, Server &server, std::vector<Client*>::iterator it);

		void	closeFd();
		void	addClientsFD(int);
		int		isCommand(const char *str);
		void	sendMsgtoClient(int fd, std::string msg);
		void	sendMsgtoChannel(Server &server, Client *client, std::string channel, std::string msg);
		void	executePrivmsg(Server &server, Client *client, std::string const &command, std::string const &args);
		void	executePrivmsgCmd(Server &server, Client *client, std::string target, std::string message);

		// void	sendMessage(std::vector<Client*>::iterator it);
		void	sendMessage();
		void	displayVector(void);
		std::vector<Client*> getClients(void);
		std::map<std::string, Channel*>& getChannels(void);
		void 	addClientToChannel(Client* client);
		void    inviteClient(Client* client);
		void    addChannelToChannels(Channel &channel);
		void    displayMap(void);
		bool	isClientInChannel(std::string const &username);
		int   	getNbUsers(void) const;
		bool	isInvited(std::string const &nickname);
		std::map<std::string, std::set<int> > &getOutput(void);
		std::vector<Client*> getClientId();
		void	removeClient(std::string const &nickname);
		bool	FirstThreeCmdsTrue(std::vector<Client*>::iterator it);
		void	ping(std::vector<Client*>::iterator it);
		int 	getClientFd(std::string const &nickname);


	private:
		int	_port;
		std::string	_password;
		int _fdserver;
		int _newfdclient;
		std::vector<struct pollfd> fds;
		struct sockaddr_in sa; //* structure de donnees de la socket
		int	_fdcount;
		std::vector<Client*> idClient;
		std::string _input;
		std::map<std::string, std::set<int> > output;
		std::map<std::string, Channel*> _channels;
		int	_cmdtrue; // * pour verifier si 3 premieres cmds rentrees
};

#endif
