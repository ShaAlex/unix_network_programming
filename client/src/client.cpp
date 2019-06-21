#include <sys/socket.h>
#include <iostream>
#include <stdio.h> // to include fgets/fputs
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> // to include struct sockaddr_in
#include <unistd.h> // to include socket close method

using namespace std;

#define PORT 3000
#define BUFFERSIZE 100

int main(int argv, char** argc)
{
    char sendBuf[BUFFERSIZE];
    char recvBuf[BUFFERSIZE];
    int client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // server PORT
    serverAddr.sin_port = htons(PORT);
    // server IP
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error while connecting to server.\n");
        exit(1);
    }

    while(fgets(sendBuf, sizeof(sendBuf), stdin) != NULL)
    {
        // send message
        send(client, sendBuf, strlen(sendBuf), 0);

        if(strcmp(sendBuf, "exit\n") == 0)
        {
            break;
        }

        // receive message
        recv(client, recvBuf, sizeof(recvBuf), 0);
        fputs(recvBuf, stdout);
    }

    close(client);

    return 0;
}