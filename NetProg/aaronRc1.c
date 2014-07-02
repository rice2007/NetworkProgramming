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
	int client_socket, n;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;

	if (argc != 4) {
		printf("Incorrect args: <server IP> <port> <message>\n");
		exit(-1);
	}

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0 ) {
		perror("socket");
		exit(-1);
	}

	bzero(&Remote_Address, sizeof(Remote_Address));
	Remote_Address.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == NULL) {
		perror("gethostbyname");
		exit(-1);
	}
	if (inet_pton(AF_INET, argv[1], &Remote_Address.sin_addr) == 0) {
		printf("Invalid network address\n");
		exit(-1);
	} else if ((inet_pton(AF_INET, argv[1], &Remote_Address.sin_addr) < 0)) {
		perror("inet_pton");
		exit(-1);
	}
	Remote_Address.sin_port = htons(atoi(argv[2]));

	connect(client_socket, (struct sockaddr *) &Remote_Address, sizeof(Remote_Address));
	printf("Connection successful\n");
	bzero(buffer, 1024);
	strcpy(buffer, argv[3]);

	while(1) {
		select(1, client_socket, client_socket, NULL, NULL);
		n = write(client_socket, buffer, sizeof(buffer));
		bzero(buffer, 1024);
		n = read(client_socket, buffer, sizeof(buffer));
		if (n == 0) {
			printf("Connection to server has been terminated.\n");
			exit(0);
		}
		printf("Server msg: %s\n", buffer);
		fgets(buffer, sizeof(buffer), stdin);
	}
	
	exit(0);
}