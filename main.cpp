#include "../include/Server.hpp"
#include "../include/Channel.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	else
	{
		Server	server;
		Channel chan;
		std::cout << "----- Server -----" << std::endl;

		try
		{
			server.setSignal();
			server.checkArgs(argv);
			server.initServer(server);
			server.checkPoll(server);

		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		std::cout << "The server is closed" << std::endl;
		server.closeFd();
		server.freeChannel();
		server.freeClients();
		close(server.getFD());
	}
	return (0);
}
