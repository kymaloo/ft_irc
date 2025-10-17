// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

bool    isNum(std::string str)
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

int main(int argc, char** argv)
{
    std::string sPort = argv[1];
    if (argc != 2 || sPort.size() != 4 || isNum(sPort) == false)
        return 1;
    int port = atoi(sPort.c_str());

    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

    // sending data
    // const char* message;
    std::string message;
    while (message != "exit")
    {
        getline(std::cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
    }

    // closing socket
    close(clientSocket);

    return 0;
}