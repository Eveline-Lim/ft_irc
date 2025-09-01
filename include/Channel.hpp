#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <set>

class Client;

class Channel
{
	public:
		Channel();
		Channel(std::string const &name);
		Channel(Channel const &obj);
		Channel	&operator=(Channel const &obj);
		~Channel();

		//Getters
		std::string const	&getName(void) const;
		std::string const	&getPassword(void) const;
		std::string	const	&getTopic(void) const;
		bool	isInviteOnly(void) const;
		bool	isTopicRestricted(void) const;
		int	getLimit(void) const;
		int	getStatus(void) const;
		int	getNbUsers(void) const;

		//Setters
		void	setName(std::string &name);
		void	setPassword(std::string &password);
		void	setTopic(std::string const &topic);
		void	setLimit(int &limit);
		void	setInvited(bool isInvited);
		void	setStatus(int status);

		bool	isOperator(std::string const &nickname);
		bool	isInvited(std::string const &client);
		bool	isClientInChannel(std::string const &nickname);
		void	addClientToChannel(Client* client);
		bool	isClientInInvited(std::string const &nickname);
		void	addInvitedClient(Client *client);
		void	removeClientFromChannel(std::string const &nickname);
		void	removeClientFromInvited(std::string const &nickname);
		std::vector<std::string>& getInvitedChannels(void);
		std::set<int>	noMsgforme(Client *client);
		std::map<std::string, Client*> &getClientList()
		{
			return (_clients);
		}
		std::string	getClientsList(void);

		//DEBUG
		void	displayMap(void);

		// Modes
		bool	modeI(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg);
		bool	modeK(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg, std::string &password);
		bool	modeL(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg, int &limit);
		bool	modeO(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg, std::string &targetUser);
		bool	modeT(std::string const &client, std::map<std::string, Channel*>::iterator ite, std::string const &arg);

	private:
		std::string	_name;
		std::string	_password;
		std::string	_topic;
		bool	_topicRestrictions;
		bool	_isInviteOnly;
		int	_userLimit;
		int	_status;

		std::map<std::string, Client*>	_clients;
		std::vector<std::string>	_invited;
};

#endif
