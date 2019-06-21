#include <sys/socket.h>
#include <iostream>
#include <stdio.h> // to include fgets/fputs
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> // to include struct sockaddr_in
#include <unistd.h> // to include socket close method
#include <sys/types.h> // to include pid_t

using namespace std;

#define PORT 3000
#define BUFFERSIZE 100
#define LISTENMQ 10

int main(int argv, char** argc)
{
    char sendBuf[BUFFERSIZE];
    char recvBuf[BUFFERSIZE];
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    int conn;
    socklen_t clientLen = sizeof(clientAddr);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // server PORT
    serverAddr.sin_port = htons(PORT);
    // server IP
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(server, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error while binding IP to server.\n");
        exit(1);
    }

    if(listen(server, LISTENMQ) == -1)
    {
        printf("Error while listening.\n");
        exit(1);
    }

    if((conn = accept(server, (struct sockaddr *)&clientAddr, &clientLen)) < 0)
    {
        printf("Error while listening.\n");
        exit(1);
    }

    while(1)
    {
        memset(recvBuf, 0, sizeof(recvBuf));

        int len = recv(conn, recvBuf, sizeof(recvBuf), 0);
        if(strcmp(recvBuf, "exit\n") == 0)
        {
            break;
        }
        fputs(recvBuf, stdout);

        send(conn, recvBuf, len, 0);
    }

    close(server);
    close(conn);
    return 0;
}
