#include "../include/Server.hpp"

void	Server::sigintHandler(int signal)
{
	std::cout << "Signal received" << std::endl;
	if (signal == SIGINT)
	{
		signalGlobal = 1;
		std::cout << "Exiting server by using ctrl+c" << std::endl;
		// close (STDIN_FILENO);
		// exit(0);
	}
	else
	{
		return ;
	}
}

void	Server::setSignal(void)
{
	// Declaration of the sigaction structure
	struct	sigaction	act;

	// We set all the structure's bits to 0
	bzero(&act, sizeof(act));
	act.sa_handler = sigintHandler;
	sigaction(SIGINT, &act, NULL);
}
