#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

	char buffer[1024];
	int server_socket, connfd;
	struct sockaddr_in serv_addr;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		printf("server sock: %d\n", server_socket);
	}
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(10550); //Listening port

	bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(server_socket, 5);
	
	while(1) {
		connfd = accept(server_socket, (struct sockaddr*) NULL, NULL);
		if (connfd < 0) {
			printf("Cannot connect");
		}
		int length = read(connfd, buffer, 1024);
		if (length < 0) {
			printf("errno %d\n", errno);
			printf("%d\n", length);
		}
		printf("Client message: %s\n", buffer);
		write(connfd, buffer, sizeof(buffer));
		close(connfd);
	}
	close(server_socket);

}
