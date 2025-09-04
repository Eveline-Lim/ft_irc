#include "../../include/Invite.hpp"

Invite::Invite()
{
	;
}
Invite::Invite(Invite const &obj) : ACommand()
{
	(void)obj;
}

Invite& Invite::operator=(Invite const &obj)
{
	(void)obj;
	return (*this);
}

Invite::~Invite()
{
	;
}

void Invite::execute(Server &server, std::string const &command, std::vector<Client*>::iterator it, std::string const &args)
{
	std::cout << "Entering " << command << " command" << std::endl;
	std::stringstream ss(args);
	std::string word;
	int count = 0;
	std::set<int> fds;
	fds.insert((*it)->getFd());
	std::map<std::string, std::set<int> > &output = server.getOutput();

	while (ss >> word)
	{
		count++;
	}
	if (count < 2)
	{
		output[ERR_NEEDMOREPARAMS((*it)->getNick())].insert((*it)->getFd());
		return ;
	}

	ss.clear();
	ss.seekg(0); // ss.str(args);
	std::string targetNickname;
	std::string channelName;
	ss >> targetNickname >> channelName;
	std::cout << "targetNickname: " << targetNickname << std::endl;
	std::cout << "channelName: " << channelName << std::endl;
	if (!((*it))->tryJoinChannel())
	{
		output[ERR_NOTREGISTERED((*it)->getNick())].insert((*it)->getFd());
		return ;
	}
	if (targetNickname.size() > 10)
	{
		std::cout << "NICK: max length for targetNickname is 9\n";
		return ;
		// ! return here, must nt go in the setnick function
	}
	for (unsigned int i = 0; i < targetNickname.size(); i++)
	{
		if (targetNickname[i] == ' ' || targetNickname[i] == ' ' || targetNickname[i] == ',' || targetNickname[i] == '*' || targetNickname[i] == '?' ||
			targetNickname[i] == '!' || targetNickname[i] == '@' || targetNickname[i] == '.' || targetNickname[i] == '$' || targetNickname[i] == ':')
		{
			output.insert(std::pair<std::string, std::set<int> >(ERR_ERRONEUSNICKNAME((*it)->getNick(), targetNickname), fds));
			return ;
		}
	}
	// if ((*it)->getUse() == false)
	// {
	// 	(*it)->setNick(nickname); // * pas besoin
	// 	// If used after registration, the server will return a NICK message
	// }

	// if (checkNickname(nickname) == true)
	// {
	// 	nckn.push_back(nickname);
	// 	std::cout << "nickname changed to : " << nickname << std::endl;
	// }
	// if (_used == true)
	// 	std::cout << ERR_NICKNAMEINUSE(nickname, nickname);
	// vector de nickname pou le comparer entre eux et verifier qu'il n'y a aps de doublons
	// 	if (channel.isInvited(user))
	// 	{
	// 		std::cerr << "Error: " << user << " already invited" << std::endl;
	// 		return;
	// 	}
	// 	channel.setInvited(user);

	// Verifier si le channel existe
	std::map<std::string, Channel*> &channels = server.getChannels();
	std::map<std::string, Channel*>::iterator ite = channels.find(channelName);

	if (ite == channels.end())
	{
		output[ERR_NOSUCHCHANNEL((*it)->getNick(), channelName)].insert((*it)->getFd());
		return;
	}

	 Channel *channel = ite->second;
	// Verifier que l’emetteur est operateur du channel
	if (!channel->isOperator((*it)->getNick()))
	{
		output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)].insert((*it)->getFd());
		return;
	}

	// Vérifier opérateur si channel est en mode +i
	// if (channel->isInviteOnly() && !channel->isOperator((*it)->getNick()))
	// {
		// output[ERR_CHANOPRIVISNEEDED((*it)->getNick(), channelName)] = fds;
		// return;
	// }
	// Verifier que le client cible existe dans le serveur
	std::vector<Client*> clients = server.getClients();
	Client *targetClient = NULL;

	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getNick() == targetNickname)
		{
			targetClient = clients[i];
			break;
		}
	}

	if (!targetClient)
	{
		output[ERR_NOSUCHNICK((*it)->getNick(), targetNickname)].insert((*it)->getFd());
		return;
	}
	std::cout << "							targetclient: " << targetClient->getNick() << std::endl;
	std::cout << "				channel:" << channel->getName() << std::endl;
	std::cout << "		nom channel: " << channelName << std::endl;
	channel->addClientToChannel((targetClient));
	// Ajouter le client a la liste des invites du channel
	channel->addInvitedClient(targetClient);
	output[RPL_INVITING((*it)->getNick(), targetClient->getNick(), channelName)].insert((*it)->getFd());
	output[RPL_INVITING((*it)->getNick(), targetClient->getNick(), channelName)].insert(targetClient->getFd());
}
