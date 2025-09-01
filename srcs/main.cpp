#include "../include/Server.hpp"

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
			//server.closeFds();
			std::cerr << e.what() << std::endl;
		}
		std::cout << "The server is closed" << std::endl;
		server.closeFd();
		// ! ne pas oublier de free ! 
	}
	return (0);
}
