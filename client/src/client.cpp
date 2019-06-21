#include <sys/socket.h>
#include <iostream>
#include <stdio.h> // to include fgets/fputs
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> // to include struct sockaddr_in
#include <unistd.h> // to include socket close method

using namespace std;

#define PORT 3000
#define BUFFERSIZE 1024

int main(int argv, char** argc)
{
    fd_set rfds;
    char recvBuf[BUFFERSIZE];
    char sendBuf[BUFFERSIZE];
    int maxfd, retval;
    struct timeval tv;
    int client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    int len;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // server PORT
    serverAddr.sin_port = htons(PORT);
    // server IP
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error while connecting to server.\n");
        exit(1);
    }

    while(1)
    {
        // clear bits in rfds
        FD_ZERO(&rfds);
        // insert stdin to rfds
        FD_SET(0, &rfds);
        // insert socket to rfds
        FD_SET(client, &rfds);

        maxfd = 0;
        if(maxfd < client)
        {
            maxfd = client;
        }

        // set the timeout value
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        // waiting chat
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if(retval == -1)
        {
            printf("Error while select.\n");
            break;
        }
        else if(retval > 0)
        {
            if(FD_ISSET(client, &rfds))
            {
                len = recv(client, recvBuf,sizeof(recvBuf), 0);
                fputs(recvBuf, stdout);
                if(strcmp(recvBuf, "exit\n") == 0)
                {
                    exit(0);
                }
                memset(recvBuf, 0, sizeof(recvBuf));
            }

            if(FD_ISSET(0, &rfds))
            {
                fgets(sendBuf, sizeof(sendBuf), stdin);
                send(client, sendBuf, strlen(sendBuf), 0);
                if(strcmp(sendBuf, "exit\n") == 0)
                {
                    exit(0);
                }
                memset(sendBuf, 0, sizeof(sendBuf));
            }
        }
        else
        {
            continue;
        }
    }

    close(client);

    return 0;
}