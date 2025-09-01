#ifndef REPLIES_HPP
# define REPLIES_HPP

// #define CRLF "\r\n

#define user_id(nickname, username, ip_address) std::string(nickname) + "!~" + std::string(username) + "@" + std::string(ip_address)

#define RPL_WELCOME(nickname, username, ip_address) ":server 001 " + std::string(nickname) + " :Welcome to the LES LOUTRES IRC Network, " + user_id(nickname, username, ip_address) + "\r\n"
#define RPL_YOURHOST(nickname) ":server 002 " + std::string(nickname) + " :Your host is LES LOUTRES, running version 1.0.0\r\n"
#define RPL_CREATED(nickname) ":server 003 " + std::string(nickname) + " :This server was created " + getTimeOfCreation()
#define RPL_MYINFO(nickname, servername, version, chanmodes) ":server 004 " + std::string(nickname) + " :" + std::string(servername) + " " + std::string(version) + " " + std::string(chanmodes) + "\r\n"
#define RPL_SUPPORT(nickname, client, modes) ":server 005 " + std::string(nickname) + " :" + std::string(client) + " " + std::string(modes) + " " + ":are supported by this server\r\n"

#define RPL_CAPLS ":Server CAP * LS :multi-prefix sasl\r\n"
#define RPL_CAPREQ(args) ":Server CAP * ACK " + args + "\r\n"

#define	RPL_JOIN(nickname, username, cmd, channel) ":" + nickname + "!~" + username + "@server " + cmd + " " + channel + "\r\n"
#define RPL_ENDOFNAME(nickname, channel) "366 " + std::string(nickname) + " " + std::string(channel) + " :End of /NAMES list\r\n"
#define RPL_NAMREPLY(nickname, channel, userlist) "353 " + std::string(nickname) + " = " + std::string(channel) + " :" + userlist + "\r\n"
#define RPL_TOPIC(nickname, channel, topic) "332 " + std::string(nickname) + " " + std::string(channel) + " :" + std::string(topic) + "\r\n"


// RPL PAS A JESSICA
// #define RPL_JOIN(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN #" + channelname + CRLF)
// #define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
// #define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)
// #define RPL_TOPIC(nickname, channelname, topic) (": 332 " + nickname + " #" +channelname + " :" + topic + "\r\n")


#define	RPL_PART(nickname, username, cmd, channel, reason) ":" + nickname + "!~" + username + "@server " + cmd + " " + channel + " " + reason + "\r\n"
#define	RPL_NICK(oldnickname, username, cmd, nickname) ":" + oldnickname + "!~" + username + "@server " + cmd + " " + nickname + "\r\n"

#define RPL_YOUREOPER(client) "381 " + std::string(client) + " :You are now an IRC operator\r\n"
#define RPL_INVITING(nick, target, channel) "341 " + std::string(nick) + " " + target + " :" + channel + "\r\n"
#define RPL_NOTIFICATION(nick, username, target, channel) ":" + nick + "!~" + username + "@server INVITE " + target + " :" + channel + "\r\n"
#define RPL_KICK(nick, username, channel, target, reason) ":" + nick + "!~" + username + "@server KICK " + channel + " " + target + " " + reason + "\r\n"
#define RPL_MODE(nick, username, channel, chain) ":" + nick + "!~" + username + "@server MODE " + channel + " " + chain + "\r\n"
#define RPL_PRIVMSG(nick, username, target, message)  ":" + nick + "!~" + username + "@server PRIVMSG " + target + " " + message + "\r\n" //* ok

#define RPL_CHGENICK(nickname) (std::string)":" + " you have changed your nickname to " + std::string(nickname) + "\r\n"

#endif



/*
std::string joinMsg = ":" + (*it)->getNick() + "!~" + (*it)->getUser() + "@host JOIN :" + chan->getName() + "\r\n";
std::string topicMsg = "332 " + (*it)->getNick() + " " + chan->getName() + " :" + chan->getTopic() + "\r\n";
std::string namesMsg = "353 " + (*it)->getNick() + " = " + chan->getName() + " :" + chan->getUserListString() + "\r\n";
std::string endOfNamesMsg = "366 " + (*it)->getNick() + " " + chan->getName() + " :End of /NAMES list\r\n";

output[joinMsg].insert(chan->getAllFds().begin(), chan->getAllFds().end());
output[topicMsg].insert(fds.begin(), fds.end());
output[namesMsg].insert(fds.begin(), fds.end());
output[endOfNamesMsg].insert(fds.begin(), fds.end());

*/
