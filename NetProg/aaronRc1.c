#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	char buffer[1024];
	int client_socket, maxfd, n;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;
	fd_set rset;
	FILE *infd;

	if (argc <= 2) {
		printf("Incorrect args: <server IP> <port>\n");
		exit(EXIT_FAILURE);
	}

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0 ) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	bzero(&Remote_Address, sizeof(Remote_Address));
	Remote_Address.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == NULL) {
		perror("gethostbyname");
		exit(EXIT_FAILURE);
	}
	if (inet_pton(AF_INET, argv[1], &Remote_Address.sin_addr) == 0) {
		printf("Invalid network address\n");
		exit(EXIT_FAILURE);
	} else if ((inet_pton(AF_INET, argv[1], &Remote_Address.sin_addr) < 0)) {
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	Remote_Address.sin_port = htons(atoi(argv[2]));

	FD_ZERO(&rset);
	n = connect(client_socket, (struct sockaddr *) &Remote_Address, sizeof(Remote_Address));
	if (n < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Connection successful\n");
	printf("Enter <command> [args]:\n");
	bzero(buffer, 1024);
	fgets(buffer, sizeof(buffer), stdin);

	while(1) {
		FD_SET(client_socket, &rset);
		FD_SET(fileno(infd), &rset);
		maxfd = client_socket;
		if(select(maxfd + 2, &rset, NULL, NULL, NULL) < 0) {
			perror("select");
		}
		if (FD_ISSET(client_socket, &rset)) {
			n = read(client_socket, buffer, sizeof(buffer));
			if (n == 0) {
				printf("Connection to server has been terminated.\n");
				exit(EXIT_SUCCESS);
			} else if (n < 0 ) {
				perror("read");
				exit(EXIT_FAILURE);
			}
			printf("Msg from server: %s", buffer);
		}
		if (FD_ISSET(fileno(infd), &rset)) {
			n = write(client_socket, buffer, sizeof(buffer));
			if (n < 1) {
				break;
			} else if (n < 0) {
				perror("write");
				exit(EXIT_FAILURE);
			}
			bzero(buffer, 1024);
			fgets(buffer, sizeof(buffer), stdin);
		}
	}
	exit(EXIT_SUCCESS);
}