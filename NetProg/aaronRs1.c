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


int main(int argc, char *argv[]) {

	char buffer[1024];
	fd_set fds;
	int listenfd, connfd;
	// int nread, nready, ns1, ns2, reader;
	struct sockaddr_in serv_addr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("server sock: %d\n", listenfd);
	}
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(10500); //Listening port

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 2);

	// if ((ns1 = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
	// perror("accept ns1");
	// exit(1);
	// }

	// /* Accept another connection. */
	// if ((ns2 = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
	// perror("accept ns2");
	// exit(1);
	// }


	// while(1) {
	// 	FD_ZERO(&fds);
	// 	FD_SET(ns1, &fds);
	// 	FD_SET(ns2, &fds);
	// 	reader = select(3, &fds, 0, 0, 0);
	// 	if ( FD_ISSET(ns1, &fds)) {
	// 		nread = recv(ns1, buffer, sizeof(buffer), 0);
	// 		if (nread < 1) { //Error checking
	// 			printf("Read err\n");
	// 			close(ns1);
	// 			close(ns2);
	// 			exit(-1);
	// 		} 
	// 		send(ns2, buffer, nread, 0);
	// 	}
	// 	if ( FD_ISSET(ns2, &fds)) {
	// 		nread = recv(ns2, buffer, sizeof(buffer), 0);
	// 		if (nread < 1) {
	// 			close(ns1);
	// 			close(ns2);
	// 			exit(-1);
	// 		}
	// 		send(ns1, buffer, nread, 0);
	// 	}


	// }
	connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
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
	close(listenfd);

}
