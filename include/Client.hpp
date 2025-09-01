#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Channel.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <errno.h>
#include <netdb.h> // * acceder a struct hostent (infos machines hote)
#include <netinet/in.h> // * library for ipv6
#include <poll.h>
#include <string>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <algorithm>

enum e_channelRole
{
	OPERATOR,
	MEMBER
};

class Channel;

class Client
{
	public:
		Client();
		Client(int fd);
		Client(Client const &obj);
		Client	&operator=(Client const &obj);
		~Client();

		int const &getFd(void) const;
		std::string const &getNick() const;
		std::string const &getUser() const;
		std::string const &getPass() const;
		std::string const &getReal() const;
		std::string const &getIp() const;
		int &getStatus(void);
		bool getUse();
		bool getIsNicknameSet(void);
		// std::vector<std::string>&displayClientsInChannels(void);

		void setNick(std::string);
		void setUser(std::string);
		void setPass(std::string);
		void setReal(std::string);
		void setIp(std::string);
		void setUse(bool used);
		void setFd(int fd);
		void setStatus(int status);
		void setInput(char buffer[1024]);
		void setIsNicknameSet(bool isSet);

		bool tryRegister(void);
		bool isRegistered(void);
		bool tryJoinChannel(void);
		bool getInput(std::string &);

	private:
		int _fd_client;
		std::string	_nickname;
		std::string _username;
		std::string _realname;
		std::string _password;
		std::string _ip_add;
		int _status; //* si admin ou pas (membres ou operator)
		bool	_used;
		bool	_registered;
		bool	_isNicknameSet;
		std::vector<char> in;
};

#endif
