#include "includes/server.hpp"
#include "includes/client.hpp"
#include "includes/Command.hpp"
#include <string.h>
#include <csignal>
#include <cstdlib>

// Variable globale pour les signaux
volatile sig_atomic_t should_exit = 0;

// Gestionnaire de signal
void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        std::cout << "\nCtrl+C détecté - fermeture gracieuse..." << std::endl;
        should_exit = 1;
    }
    else if (sig == SIGQUIT)
    {
        std::cout << "\nCtrl+\\ détecté - arrêt immédiat..." << std::endl;
        exit(1);
    }
}

/*Returns true if the whole string is digital.
Does not check if it fits in an int.
Can start with + or -.*/
bool	isNum(std::string str)
{
	int i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (std::isdigit(str[i]) == false)
			return false;
		i++;
	}
	return true;
}

/*Accepts new incoming clients.
Returns true if accept() fails.*/
bool acceptNewClients(Server& serv)
{
	bool end = false;
	int rv = 0;

	while (rv != -1)
	{
		rv = serv.setNewClient();
		if (rv < 0)
			end = true;		
		break ;
	}
	serv.unsetRevent(0);
	return end;
}

/*Tries to receive from a client.
Then tries to send to every client except itself.*/
bool communicate(Server& serv, int it)
{
	// char*	buffer = new char[1024];
	// bzero(buffer, 1024);

	if (serv.receiveClient(it) == -1)
	{
		serv.closeFd(it);
		// delete []buffer;
		return true;
	}
	// delete []buffer;
	return false;
}

int main(int argc, char**argv)
{
	struct sigaction sa;
	bool		end, compress = false;
	int			rv, port = -1;
	std::string	sPort = argv[1];
	Server		serv;

	sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

	if (argc != 3 || isNum(sPort) == false)
	{
		std::cerr << "Usage: ./ircserv <port> <password>\n";
		return 1;
	}
	port = atoi(sPort.c_str());
	serv.setPass(argv[2]);
	serv.setUpServer(port, 5);
	while (!should_exit)
	{
		do
		{
			compress = false;

			rv = poll(serv.getPfds(), serv.getNumberFds(), -1);
			if (rv < 0)
			{
				std::cerr << "  poll() failed\n";
				break;
			}
			for (int i = 0; i < serv.getNumberFds(); i++)
			{
				if (serv.getRevents(i) == 0)
					continue ;
				if (!(serv.getRevents(i) & POLLIN))
					break ;
				if (i == 0)
					end = acceptNewClients(serv);
				else
				{
					compress = communicate(serv, i);
					serv.redirect(serv.getClientfd(i));
					serv.emptyBuffer();
				}
			}
			if(compress == true)
				serv.compressArray();
		} while (end == false);
	}
	std::cout << "Serveur fermé." << std::endl;
    return 0;
}
