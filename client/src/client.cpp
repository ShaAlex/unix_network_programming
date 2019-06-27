#include <sys/socket.h>
#include <iostream>
#include <stdio.h> // to include fgets/fputs
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h> // to include struct sockaddr_in
#include <unistd.h> // to include socket close method
#include <pthread.h>
#include <errno.h>

using namespace std;

#define PORT 3000
#define BUFFERSIZE 1024

void* recvSocket(void *ptr)
{
    pthread_detach(pthread_self());
    int fd = *(int *)ptr;
    char str[BUFFERSIZE];

    while(1)
    {
        memset(str, 0, sizeof(str));

        int bytes = recv(fd, str, sizeof(str), 0);
        if(bytes <= 0)
        {
            break;
        }

        if(strcmp(str, "exit\n") == 0)
        {
            break;
        }
        printf("%s", str);
    }

    exit(0);
    return NULL;
}

void* sendSocket(void *ptr)
{
    pthread_detach(pthread_self());
    int fd = *(int *)ptr;
    char str[BUFFERSIZE];

    while(1)
    {
        memset(str, 0, sizeof(str));
        // get input from stdin
        read(STDIN_FILENO, str, sizeof(str));
        if(strcmp(str, "exit\n") == 0)
        {
            break;
        }
        // send message to server
        send(fd, str, strlen(str), 0);
    }

    exit(0);
    return NULL;
}

int main(int argv, char** argc)
{
    int client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    pthread_t thread1, thread2;
    void *status1, *status2;

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

    pthread_create(&thread1, NULL, recvSocket, &client);
    pthread_create(&thread2, NULL, sendSocket, &client);

    // pthread_join(thread1, &status1);
    // pthread_join(thread2, &status2);
    pthread_exit(0);

    close(client);

    return 0;
}