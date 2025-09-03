#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/ACommand.hpp"
#include "../include/Pass.hpp"
#include "../include/User.hpp"
#include "../include/Nick.hpp"
#include "../include/Join.hpp"
#include "../include/Kick.hpp"
#include "../include/Invite.hpp"
#include "../include/Topic.hpp"
#include "../include/Mode.hpp"
#include "../include/Part.hpp"
#include "../include/Quit.hpp"
#include "../include/Errors.hpp"
#include "../include/Replies.hpp"
#include "../include/Privmsg.hpp"

bool	signalGlobal = 0;

Server::Server() : _port(0), _password(""), _fdserver(0), _newfdclient(0), _fdcount(0), idClient()
{

}

Server::Server(Server const &obj)
{
	this->_port = obj._port;
	this->_password = obj._password;
	this->_fdserver = obj._fdserver;
	this->_newfdclient = obj._newfdclient;
	this->_fdcount = obj._fdcount;

}

Server &Server::operator=(Server const &obj)
{
	this->_port = obj._port;
	this->_password = obj._password;
	this->_fdserver = obj._fdserver;
	this->_newfdclient = obj._newfdclient;
	this->_fdcount = obj._fdcount;
	return (*this);
}

Server::~Server()
{
	//* gerer les fermeture de fd
}

std::string const &Server::getPassword(void) const
{
	return (this->_password);
}

void	Server::initServer(Server &server)
{
	(void)server;
	struct pollfd pollServer;
	memset(&sa, 0, sizeof(sa));
	int opt = 1;

	_fdserver = socket(AF_INET, SOCK_STREAM, 0);
	if (_fdserver  == -1)
	{
		std::cout << "socket failed : " << strerror(errno) << std::endl;
		return ;
	}
	if (setsockopt(_fdserver, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		std::cout << "error : " << strerror(errno) << std::endl;
		close(_fdserver);
		return ;
	}
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sa.sin_port = htons(_port);
	if (bind(_fdserver, (struct sockaddr*)&sa, sizeof sa) == -1)
	{
		std::cout << "bind failed : " << strerror(errno) << std::endl;
		close(_fdserver);
		return ;
	}
	if (listen(_fdserver , SOMAXCONN) == -1) //* backlog egal au nombre de fd poll ?
	{
		std::cout << "listen failed : " << strerror(errno) << std::endl;
		close(_fdserver);
		return ;
	}
	std::cout << "Listen on FD SERVER: " << _fdserver<< std::endl;
	memset(&fds, 0, sizeof(fds));
	pollServer.fd = _fdserver;
	pollServer.events = POLLIN;
	pollServer.revents = 0;
	fds.push_back(pollServer);
	// _fdcount++;
}


bool Server::checkPoll(Server &server)
{
	(void)server;
	std::cout << "POLL FD : " << fds[0].fd << std::endl;
	std::cout << "Waiting on poll()...\n";
	char buffer[1024];
	int	read_bytes;
	while (signalGlobal == 0) // ? boolean ?
	{
		// std::cerr<<"coucou le poll"<<std::endl;
		int poll_count = poll(fds.data(), fds.size(), 0);
		// std::cerr<<"au revoir le poll"<<std::endl;
		if (poll_count < 0)
		{
			std::cout << "poll failed : " << strerror(errno) << std::endl;
			// closeFd();
			// return (false);
		}
		for (size_t i = 0; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == _fdserver)
				{
					std::cout << "FDS = " << fds[i].fd << std::endl;
					newClient();
					std::cout << "Listening socket is readable\n";
				}
				else
				{
					memset(buffer, 0, sizeof(buffer));
					read_bytes = 0;
					std::cout << "RECV here\n";
					read_bytes = recv(fds[i].fd, buffer, sizeof(buffer), 0); // * 1er arg clientfd
					std::cout << "buffer = " << buffer << std::endl;
					if (read_bytes == 0)
					{
						std::cout << "client closed\n"; // QUIt et delete clients
						std::cout << "FD SIZE = " << idClient.size() << std::endl;
						std::cout << "I ========= " << i << std::endl;
						for(size_t p = 0; p < idClient.size();p++)
						{
							if (idClient[p]->getFd() == fds[i].fd)
							{
								closeOneClient(idClient.begin() + p);
								server.closeOnefds(fds[i].fd);
							}
						}
					}
					if (read_bytes == -1)
					{
						std::cout << "recv failed : " << strerror(errno) << std::endl;
						closeFd();
					}
					else
					{
						std::cout<<"?????????????????????????????????????????????"<<std::endl;
						for (std::vector<Client*>::iterator it = idClient.begin(); it != idClient.end(); it++)
						{
							if ((*it)->getFd() == fds[i].fd)
							{
								std::cout << "BUFFFFFFFFFFFFFFF = " << buffer << std::endl;
								(*it)->setInput(buffer);
								std::string res;
								if ((*it)->getInput(res) == true)
									executeCommands((char *)res.c_str(), server, it);
							}
						}
					}
					// ? boolean for connection closed = false if error ?
				}
			}
		}
		sendMessage();
		for (std::vector<Client*>::iterator it = idClient.begin(); it != idClient.end(); it++)
		{
			std::string res;
			// std::cerr<<"on traite les infos"<<std::endl;
			if ((*it)->getInput(res) == true)
				executeCommands((char *)res.c_str(), server, it);
		}
	}
	return (true);
}

void	Server::deleteClients(int index)
{
	fds[index] = fds[_fdcount - 1];
	// std::cout << "DELETE FDS : " << fds[index].fd << std::endl;
	_fdcount--;
	// std::cout << "FD COUNT-- " << _fdcount << std::endl;
}

void	Server::closeFd() // fermeture SEULEMENT clients ! ne pas fermer pas le fd du server
{
	std::cout << "ICI" << std::endl;
	// for (unsigned int i = 0; i < idClient.size() ;i++)
	// {
	// 	close(idClient[i]->getFd());
	// }
	for (std::vector<Client *>::iterator it = idClient.begin(); it != idClient.end(); ++it)
	{
		close((*it)->getFd());
	}
}

void	Server::closeOneClient(std::vector<Client*>::iterator it)
{
		close((*it)->getFd());
		// free (*it);
		idClient.erase(it);
}

void	Server::freeClients()
{
	for (std::vector<Client *>::iterator it = idClient.begin(); it != idClient.end(); ++it)
	{
		if (*it)
			delete (*it);
	}
}

void	Server::freeChannel()
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second)
			delete (it->second);
	}
}

void	Server::closeAllfd()
{
	// for (unsigned int i = 1; i < fds.size() ;i++)
	// {
	// 	close(fds[i].fd);
	// }
	for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
	{
		// if (it->fd == clientFd)
		// {
			it = fds.erase(it);
			break;
		// }
	}

}

void	Server::closeOnefds(int fd)
{
	// for (unsigned int i = 1; i < fds.size() ;i++)
	// {
	// 	close(fds[i].fd);
	// }
	for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			it = fds.erase(it);
			break;
		}
	}

}

void	Server::newClient()
{
	std::cout << "NEW CLIENT\n";
	struct sockaddr_in client_addr;
	socklen_t addr_size = sizeof(client_addr);
	struct pollfd clientpoll;

	_newfdclient = accept(_fdserver, (struct sockaddr *)&client_addr, &addr_size);
	// * valeur de retour d'accept est le fd client et l relier au vector du client
	std::cout << "CLIENT FD : " << _newfdclient << std::endl;
	// fcntl(_newfdclient, O_NONBLOCK);
	if (_newfdclient < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cout << "accept failed : " << strerror(errno) << std::endl;
			closeFd();
		}
	}
	std::string client_ip = inet_ntoa(client_addr.sin_addr);
	Client *newclient = new Client(_newfdclient);
	idClient.push_back(newclient);
	newclient->setIp(client_ip);
	clientpoll.fd = _newfdclient;
	clientpoll.events = POLLIN;
	clientpoll.revents = 0;
	fds.push_back(clientpoll);
	std::cout << "NBD CLIENT = " << idClient.size() << std::endl;
	std::cout << "IP ADD" << newclient->getIp() << std::endl;
	std::cout << "FD COUNT FTER CLIENT" << _fdcount << std::endl;
	// ! Ne pas oublier de close ! ne pas close dans le destructeur
}

int Server::isCommand(const char *str)
{
	int	i;
	int	commandFound;
	const char *command[] = {"PASS", "USER", "NICK", "PRIVMSG", "CAP", "JOIN", "KICK", "INVITE", "MODE", "TOPIC", "PART", "QUIT", "PING", NULL};

	i = 0;
	commandFound = -1;
	while (command[i] != NULL)
	{
		if (strcmp(command[i], str) == 0)
		{
			commandFound = i;
			std::cout << "command found: " << commandFound << std::endl;
			break ;
		}
		i++;
	}
	return (commandFound);
}

bool Server::executeCommands(char *buffer, Server &server, std::vector<Client*>::iterator it)
{
	std::set<int> fds;
	fds.insert((*it)->getFd());
	_input = std::string(buffer);
	std::cerr << "Inooout before: " << _input << std::endl;
	// const unsigned long pos = _input.find('\n');
	// if (pos == std::string::npos)
	// 	return (false);
	std::stringstream ss(_input);
	std::cout << "_INPUT: " << _input << std::endl;
	std::string commandName;
	std::string args;
	ss >> commandName;
	std::cout << "command: " << commandName << std::endl;
	if (ss.fail())
	{
		std::cerr << "Error: stringstream failed" << std::endl;
		ss.clear();
		// close/free tout ce qui a ete alloue
		return (false);
	}
	int cmdType = isCommand(commandName.c_str());
	if (cmdType == -1)
	{
		// Ecrire le message d'erreur dans le client
		std::cerr << "Error: command " << commandName << " does not exist" << std::endl;
		commandName.clear();
		_input.clear();
		return (false);
	}
	ACommand *userCmd = NULL;
	args = _input.substr(commandName.size(), _input.length());
	switch (cmdType)
	{
		case PASS:
			std::cout << "PASS FOUND" << std::endl;
			std::cout << "ARGS: " << args << std::endl;
			userCmd = new Pass();
			userCmd->execute(server, commandName, it, args);
			break ;

		case USER:
			std::cout << "FD USER = " << (*it)->getFd() << std::endl;
			std::cout << "USER FOUND" << std::endl;
			std::cout << "ARGS: " << args << std::endl;
			userCmd = new User();
			userCmd->execute(server, commandName, it, args);
			break ;

		case NICK:
			std::cout << "NICK FOUND" << std::endl;
			std::cout << "ARGS: " << args << std::endl;
			userCmd = new Nick();
			userCmd->execute(server, commandName, it, args);
			break ;

		case PRIVMSG:
		std::cout << "ARGS: " << args << std::endl;
		userCmd = new Privmsg();
		userCmd->execute(server, commandName, it, args);
		break ;

		case CAP:
			std::cout << "CAP FOUND" << std::endl;
			output.insert(std::pair<std::string, std::set<int> >(":server CAP * LS :KICK INVITE TOPIC MODE\r\n", fds));
			break ;

		case JOIN:
			std::cout << "JOIN FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Join();
			userCmd->execute(server, commandName, it, args);
			break ;

		case KICK:
			std::cout << "KICK FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Kick();
			userCmd->execute(server, commandName, it, args);
			break ;

		case INVITE:
			std::cout << "INVITE FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Invite();
			userCmd->execute(server, commandName, it, args);
			break ;

		case TOPIC:
			std::cout << "TOPIC FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Topic();
			userCmd->execute(server, commandName, it, args);
			break ;

		case MODE:
			std::cout << "MODE FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Mode();
			userCmd->execute(server, commandName, it, args);
			break ;

		case PART:
			std::cout << "PART FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Part();
			userCmd->execute(server, commandName, it, args);
			break ;

		case QUIT:
			std::cout << "QUIT FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			userCmd = new Quit();
			userCmd->execute(server, commandName, it, args);
			break ;

		case PING:
			std::cout << "PING FOUND" << std::endl;
			std::cout << "args: " << args << std::endl;
			ping(it);
			break;

		default:
			output.insert(std::pair<std::string, std::set<int> >("Error: command does not exist", fds));
			commandName.clear();
			_input.clear();
			return (false);
	}
	commandName.clear();
	_input.clear();
	// FirstThreeCmdsTrue(it);
	delete (userCmd);
	/* *** VERIFICATION *** */
	// displayVector();
	// displayMap();
	/* ******************** */
	// boolean pour RPL
	return (true);
}

void Server::displayVector(void)
{
	std::cout << "Display of clients vector" << std::endl;
	for (size_t i = 0; i < idClient.size(); i++)
	{
		std::cout << "v[" << i << "] = " << idClient[i]->getNick() << std::endl;
	}
}

std::vector<Client*> &Server::getClients(void)
{
	return (idClient);
}

int	Server::retrieveVectorSize()
{
	return (idClient.size());
}

void	Server::sendMsgtoClient(int fd, std::string msg)
{
	if (send(fd, msg.data(), msg.size(), 0) < 0)
		std::cout << "send failed : " << strerror(errno) << std::endl;
}

std::map<std::string, Channel*>& Server::getChannels(void)
{
	return (_channels);
}

void	Server::addChannelToChannels(Channel &channel)
{
	_channels[channel.getName()] = &channel;
}

// void    Server::sendMessage(std::vector<Client*>::iterator it)
void	Server::sendMessage()
{
	if (output.size() > 0)
		std::cerr<<"on envoi output : "<<output.size()<<std::endl;
	std::map<std::string, std::set<int> >::iterator msg_it;
	for (msg_it = output.begin(); msg_it != output.end(); msg_it++)
	{
		std::cerr<<"set nb fd : "<<msg_it->second.size()<<std::endl;
		for (std::set<int>::iterator setIt = msg_it->second.begin(); setIt != msg_it->second.end();setIt++)
			send(*setIt, msg_it->first.c_str(), msg_it->first.size(), 0);
		// std::cerr << "fd: " << *msg_it->second.begin() << " message: " <<msg_it->first.c_str() << std::endl;
		// break;
	}
	output.clear();
}

std::map<std::string, std::set<int> > &Server::getOutput(void)
{
	return (output);
}

std::vector<Client*> Server::getClientId()
{
	return (idClient);
}

void Server::removeClient(std::string const &nickname)
{
	std::vector<Client *>::iterator it;

	for (it = idClient.begin(); it != idClient.end(); it++)
	{
		if ((*it)->getNick() == nickname)
		{
			idClient.erase(it);
			break ;
		}
	}
}

/**
 * @brief Get the time of creation of the Sever
 *
 * @return char* A string containing the time of creation of the server
**/
char *getTimeOfCreation(void)
{
	std::time_t	result = std::time(NULL);
	return (std::asctime(std::localtime(&result)));
}

bool	Server::FirstThreeCmdsTrue(std::vector<Client*>::iterator it)
{
	std::cout << "ICI" << std::endl;
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::string response;

	if ((*it)->isRegistered() == true)
	{
		std::cout << "nick in User: " << (*it)->getNick() << std::endl;
		response += RPL_WELCOME((*it)->getNick(), (*it)->getUser(), (*it)->getIp());
		response += RPL_YOURHOST((*it)->getNick());
		response += RPL_CREATED((*it)->getNick());
		response += RPL_MYINFO((*it)->getNick(), "ircserv", "1.0", "itkol");
		// response += ":server MODE " + (*it)->getNick() + " +i\r\n";
		response += RPL_SUPPORT((*it)->getNick(), "ircserv", "itkol");
		output.insert(std::pair<std::string, std::set<int> >(response, fds));
		return (true);
	}
	return (false);
}

void	Server::ping(std::vector<Client*>::iterator it)
{
	(void)it;
	std::set<int> fds;
	fds.insert((*it)->getFd());

	output.insert(std::pair<std::string, std::set<int> >("PONG\r\n", fds));
}

void	Server::removeChannel(std::string const &chanName)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(chanName);
	std::cout << "CHANNED REMOVED" << std::endl;
	delete (it->second);
	_channels.erase(it);
}

int	Server::getFD()
{
	return(_fdserver);
}
