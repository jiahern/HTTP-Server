// A simple server in the internet domain using TCP
// The port number is passed as an argument
// To compile: gcc server.charac -o server
// Reference: Beej'string networking guide, man pages

#define _POSIX_C_SOURCE 200112L
#define IMPLEMENTS_IPV6
#define MULTITHREADED

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include "utils.h"

int main(int argc, char **argv)
{
    int sockfd, newsockfd, re, string;
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size;

    // 4 elements of command
    if (argc < 4)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(EXIT_FAILURE);
    }

    // Create address we're going to listen on (with given port number)
    memset(&hints, 0, sizeof hints);

    // Condition for IPv4 OR IPv6
    if (strcmp(argv[1], "4") == 0)
    {
        hints.ai_family = AF_INET;
    }
    else if (strcmp(argv[1], "6") == 0)
    {
        hints.ai_family = AF_INET6;
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    // hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // for bind, listen, accept
    // node (NULL means any interface), service (port), hints, res
    string = getaddrinfo(NULL, argv[2], &hints, &res);
    if (string != 0)
    {
        fprintf(stderr, "getaddrinfo: %string\n", gai_strerror(string));
        exit(EXIT_FAILURE);
    }

    // Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Reuse port if possible
    re = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(int)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // Bind address to the socket
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(res);

    // Listen on socket - means we're ready to accept connections,
    // incoming connection requests will be queued, man 3 listen
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        // Accept a connection - blocks until a connection is ready to be accepted
        // Get back a new file descriptor to communicate on
        client_addr_size = sizeof client_addr;
        newsockfd =
            accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);
        if (newsockfd < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        data_t *data = malloc(sizeof(data_t));
        assert(data);

        char *rootDirectory = argv[3];
        strcpy(data->rootPath, rootDirectory);
        data->newsockfd = newsockfd;
        pthread_t tid;

        // Deal with Multithreaded Programming for the Web Server
        pthread_create(&tid, NULL, multiProcess, (void *)data);
    }
    close(sockfd);
    return 0;
}
