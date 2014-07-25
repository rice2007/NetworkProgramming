#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	char buffer[1024];
	int client[FD_SETSIZE], connfd, listenfd, maxfd, sockfd;
	int err, i, maxi, pidStatus, selStatus;
	struct sockaddr_in client_addr, server_addr;
	fd_set rset, allset;
	socklen_t client_length;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		perror("socket");
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(10500); //listening port

	//bind to socket
	err = bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("bind");
		exit(-1);
	}
	err = listen(listenfd, 64); //listen for connections
	if (err < 0) {
		perror("listen");
		exit(-1);
	}

	// Create client array.
	maxfd = listenfd;
	maxi = INT_MIN;
	for (i = 0; i <= FD_SETSIZE; i++) {
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	/*Infinite loop that iterates through available client connections.
	select() waits for a connection or data. If a new connection in detected,
	accept() accepts it on a socket, and it is added to the client array. The 
	program then updates the number of file descriptors and connections on
	hand. The server then reads and echoes back data from and to a client. If
	a client enters end, the server shuts down.*/
	while(1) {
		rset = allset; 
		selStatus = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (selStatus < 0) {
			perror("select");
		}
		if (FD_ISSET(listenfd, &rset)) {
			client_length = sizeof(client_addr);
			connfd = accept(listenfd, (struct sockaddr*) &client_addr, &client_length);
			if (connfd < 0) {
				perror("accept");
			}
			for (i = 0; i < FD_SETSIZE; i++) 
				if (client[i] < 0) {
					client[i] = connfd;
					break;
				}

			FD_SET(connfd, &allset);
			if (connfd > maxfd) {
				maxfd = connfd;
			}
			if (i > maxi) {
				maxi = i;
			}
		}

		for (i = 0; i <= maxi; i++) {
			sockfd = client[i];
			if (FD_ISSET(sockfd, &rset)) {
				read(sockfd, buffer, sizeof(buffer));
				strcpy(*argv, buffer);
				if (strcmp(buffer, "end") == 0 || strcmp(buffer, "end\n") == 0) {
					strcpy(buffer, "Command rcd'v from client. Terminating session.");
					printf("%s\n", buffer);
					write(sockfd, buffer, sizeof(buffer));
					close(sockfd);
					exit(0);
				}
/*				*argv = &buffer[0];
				argv[1] = 0;*/
				printf("buffer:%s\n", buffer);
				printf("argv:%s\n", *argv);
				printf("argv[0]:%s\n", argv[0]);
				printf("argv[1]:%s\n", argv[1]);
				*argv = strtok_r(*argv, " ", argv);

				printf("argv:%s\n", *argv);
				printf("Msg from socket %d: %s\n", sockfd, buffer);
				write(sockfd, buffer, sizeof(buffer));
/*				*tokenArgs = strtok_r(*argv, " ", argv);
				if (tokenArgs) {
					printf("tokenIf: %s\n", *tokenArgs);
				}
				while ((*tokenArgs = strtok_r(NULL, " ", argv)) != NULL) {
					printf("tokenWhile: %s\n", tokenArgs);
				}*/
/*				int i;
				for (i = 0; i < sizeof(buffer); i++) {
					*tokenArgs = strtok_r(NULL, " ", &tokenPtr)
					if (tokenArgs == NULL) {
						break;
					}

				}
				printf("token:%s\n", *token);*/

				i = fork();
				if (i < 0) {
					//Close socket on fork error.
					perror("fork");
					exit(-1);
				} else if (i == 0) {
/*					char *argv[1024];
					strcpy(*argv[0], buffer[0]);*/
					//close(sockfd);
					execve(argv[0], argv, 0);
					exit(0);
				} else {
					waitpid(i, &pidStatus, WNOHANG);
					//close(sockfd);

				}
			}
		}
	}
	return 0;
}