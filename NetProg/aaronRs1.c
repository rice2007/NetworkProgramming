#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*void printError(int err) {
	if (err < 0) {
		perror
	}
}*/

int main(int argc, char *argv[]) {

	char buffer[1024];
	int client[FD_SETSIZE], connfd, listenfd, maxfd, sockfd;
	int err, i, maxi, nready;
	ssize_t n;
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
	server_addr.sin_port = htons(10500); //Listening port

	if ( (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
		< 0) {
		perror("bind");
	}
	if ( (listen(listenfd, 2)) < 0 ) {
		perror("listen");
	}
	maxfd = listenfd;
	maxi =-1;
	for (i = 0; i <= FD_SETSIZE; i++) {
		client[i] = -1;
	}
	if ( (FD_ZERO(&allset) < 0 ) {
		perror("FD_ZERO");
	}
	if ( (FD_SET(listenfd, &allset) < 0) {
		perror("FD_SET");
	}

	while(1) {
		rset = allset; 
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
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
			if (i == FD_SETSIZE) {
				//Client max
			}

			FD_SET(connfd, &allset);
			if (connfd > maxfd) {
				maxfd = connfd;
			}
			if (i > maxi) {
				maxi = i;
			}
			if(--nready <= 0) {
				continue;
			}
		}

		for (i = 0; i <= maxi; i++) {
			sockfd = client[i];
			if (sockfd < 0) {
				continue;
			}
			if (FD_ISSET(sockfd, &rset)) {
				n = read(sockfd, buffer, sizeof(buffer));
				printf("Msg from socket %d: %s\n", sockfd, buffer);
				if (n = 0) {
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else {
					write(sockfd, buffer, sizeof(buffer));
				}

				if (--nready <= 0) {
					break;
				}
			}
		}
	}

/*	connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
	while(1) {

		if (connfd < 0) {
			printf("Cannot connect");
		}

		int length = read(connfd, buffer, 1024);
		if (length < 0) {
			printf("errno %d\n", errno);
			printf("%d\n", length);
		}
		if (strcmp(buffer, "end") == 0) {
			printf("Command rcd'v from client. Terminating session.\n");
			exit(0);
		}
		printf("Client message: %s\n", buffer);
		write(connfd, buffer, sizeof(buffer));
		//bzero(buffer, sizeof(buffer));
		//close(connfd);
	}
	close(listenfd);*/

}
