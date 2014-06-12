#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

	char buffer[1024];
	int server_socket, connfd;
	struct sockaddr_in serv_addr;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(10551); //Listening port

	bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(server_socket, 5);

	while(1) {
		connfd = accept(server_socket, (struct sockaddr*) NULL, NULL);
		read(connfd, buffer, strlen(buffer));
		write(connfd, buffer, strlen(buffer));
		close(connfd);
		sleep(1);
	}
}
