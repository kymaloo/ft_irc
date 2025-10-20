#include "server/server.hpp"
#include "client/client.hpp"
#include <string.h>

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

int main(int argc, char**argv)
{
	int rv = -1;
	std::string sPort = argv[1];
	if (argc != 2 || isNum(sPort) == false)
		return 1;
	int port = atoi(sPort.c_str());

	Server serv;
	serv.setUpServer(port, 5);
	char* buffer = new char[1024];






	while (1)
	{
		rv = poll(serv.getPfds(), serv.getNumberFds(), -1);
		if (rv < 0)
		{
			std::cerr <<"  poll() failed\n";
			break;
		}
		



		for (int i = 0; i < serv.getNumberFds(); i++)
		{

			if (serv.getPfds()[i].revents == 0)
				continue ;
			if (serv.getPfds()[i].revents != POLLIN)
			{
				std::cout << "stupid bitch\n";				
				break ;
			}
			if (serv.getPfds()[i].fd == serv.getServSock())
			{
				do
				{
					std::cout << "boucle infini\n";
					rv = serv.setNewClient();
					if (rv < 0)
						break ;
				} while (rv != -1);
			}
			else
			{
				// while (1)
				// {
				rv = serv.receiveClient(&buffer, i);
				// serv.receiveAll(&buffer);

				// if (rv < 0)
				// 	break ;
				
				std::cout << "msg: " << buffer << std::endl;
				// }
			}
		}
	}

    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#define SERVER_PORT  12345

#define TRUE             1
#define FALSE            0

main (int argc, char *argv[])
{
int    len, rc, on = 1;
int    servSocket = -1, newSocket = -1;
int    desc_ready, end_server = FALSE, compress_array = FALSE;
int    close_conn;
char   buffer[80];
struct sockaddr_in6   addr;
int    timeout;
struct pollfd fds[200];
int    nfds = 1, current_size = 0, i, j;


fds[0].fd = servSocket;
fds[0].events = POLLIN;
timeout = (3 * 60 * 1000);



do
{
	#####                   ON REGARDE POLL
	rc = poll(fds, nfds, timeout);

	if (rc < 0)
	perror("  poll() failed");
	if (rc == 0)
	printf("  poll() timed out.  End program.\n");









	current_size = nfds;
	########			On regarde tous les fds
	for (i = 0; i < current_size; i++)
	{

		#######			Si rien on i++ ==>
		if(fds[i].revents == 0)
			continue;


		###########		Si c'est pas POLLIN on lance une erreur.
		if(fds[i].revents != POLLIN)
		{
			printf("  Error! revents = %d\n", fds[i].revents);
			end_server = TRUE;
			break;

		}


		#########				Si on lit le socket serveur
		if (fds[i].fd == servSocket)
		{
			printf("  Listening socket is readable\n"); # (le serveur)
			
			#######		On fait des sockets clients tant que ca echoue pas
			do
			{
				newSocket = accept(servSocket, NULL, NULL);
				if (newSocket < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						perror("  accept() failed");
						end_server = TRUE;
					}
					break;
				}
				printf("  New incoming connection - %d\n", newSocket);


				##########			On enrgistre les sockets clients dans pollfd
				fds[nfds].fd = newSocket;
				fds[nfds].events = POLLIN;
				nfds++;

			} while (newSocket != -1);
		}









		############		Si on lit PAS le socket serveur
		else
		{
			printf("  Descriptor %d is readable\n", fds[i].fd);
			close_conn = FALSE;



			#######	cest un while (true)
			do
			{



				#######			on recoit tout ce qu'on peut

				rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				#################################		verifs de securite
				if (rc < 0)
				{
					if (errno != EWOULDBLOCK)
					{
					perror("  recv() failed");
					close_conn = TRUE;
					}
					break;
				}

				if (rc == 0)
				{
					printf("  Connection closed\n");
					close_conn = TRUE;
					break;
				}
				#################################		verifs de securite
				len = rc;
				printf("  %d bytes received\n", len);



				#######			on envoit tout ce qu'on recoit tout de suite
				rc = send(fds[i].fd, buffer, len, 0);
				if (rc < 0)
				{
					perror("  send() failed");
					close_conn = TRUE;
					break;
				}

			} while(TRUE);


			if (close_conn)
			{
			close(fds[i].fd);
			fds[i].fd = -1;
			compress_array = TRUE;
			}


		}
	}



	if (compress_array)
	{
		compress_array = FALSE;
		for (i = 0; i < nfds; i++)
		{
			if (fds[i].fd == -1)
			{
			for(j = i; j < nfds-1; j++)
			{
				fds[j].fd = fds[j+1].fd;
			}
			i--;
			nfds--;
			}
		}
	}

} while (end_server == FALSE);





*/