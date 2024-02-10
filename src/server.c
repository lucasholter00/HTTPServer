#include "server.h"
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

void respond(int*);
void* handleConnection(void*);

void initialize(void){
    int server_fd;
    int iSocket;
    int* pSocket;
    struct sockaddr_in addrSocket;
    int opt = 1;

    socklen_t iAddrLen = sizeof(addrSocket);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Error: Could not create socket: %s. Exiting...\n", strerror(errno));
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        printf("Error: Could not attach: %s. Exiting...\n", strerror(errno));
        exit(1);
    }

    addrSocket.sin_family = AF_INET;            // protocol
    addrSocket.sin_addr.s_addr = INADDR_ANY;    // address
    addrSocket.sin_port = htons(PORT);          // port

    if (bind(server_fd, (struct sockaddr*)&addrSocket, iAddrLen) < 0){
        printf("Error: Could not bind: %s. Exiting...\n", strerror(errno));
        exit(1);
    }

    // now we just wait and listen for the connections from clients
    // the second parameter is the number of connections to be put in queue before rejecting them
    // here, we put 1
    if ((listen(server_fd, 1)) < 0)
    {
        printf("Error: Could not listen: %s. Exiting...\n", strerror(errno));
        exit(1);
    }


    printf("Listening for requests on port: %d\n", PORT);

    pthread_t thread;

    while(1){
        // once we get a connect, we accept it
        iSocket = accept(server_fd, (struct sockaddr*)&addrSocket, &iAddrLen);

        if (iSocket < 0){
            printf("Error: Could not accept a connection: %s. Exiting...\n", strerror(errno));
            exit(1);
        }

        printf("Connection accepted\n");

        pSocket = malloc(sizeof(int));
        *pSocket = iSocket;

        pthread_create(&thread, NULL, handleConnection, (void*)pSocket);

        pthread_detach(thread);
    }

    close(server_fd);
}


void* handleConnection(void* socket){
    char strReadBuffer[1024];
    int* pSocket;
    pSocket = (int*)socket;

    if(recv(*pSocket, strReadBuffer, sizeof(strReadBuffer), 0) < 0){
        printf("Error reading message from client: %s. Exiting...\n", strerror(errno));
        exit(1);
    }

    respond(socket);

    close(*pSocket);
    free(pSocket);

    printf("Thread complete\n");
    return NULL;
}

void respond(int* iSocket){
    send(*iSocket, "Goodbye\n", 9, 0);
}

