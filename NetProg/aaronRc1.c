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

void connectWrapper(int err) {
	if (err < 0) {
		perror("connect");
	}
	exit(err);
}

int main(int argc, char *argv[]) {

	char buffer[1024];
	int client_socket, n;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;

	if (argc != 4) {
		printf("Incorrect args: <server IP> <port> <message>\n");
		exit(0);
	}

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0 ) {
		printf("errno = %d\n", errno);
		printf("Error creating socket\n");
		exit(0);
	}

	bzero(&Remote_Address, sizeof(Remote_Address));
	Remote_Address.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == NULL) {
		printf("errno = %d\n", errno);
		printf("Host does not exist\n");
		exit(0);
	}
	if (inet_pton(AF_INET, argv[1], &Remote_Address.sin_addr) == 0) {
		printf("Invalid network address\n");
		exit(0);
	} else if ((inet_pton(AF_INET, argv[1], &Remote_Address.sin_addr) < 0)) {
		printf("errno = %d\n", errno);
		printf("Invalid address family\n");
		exit(0);
	}
	Remote_Address.sin_port = htons(atoi(argv[2]));

	connectWrapper(connect(client_socket, (struct sockaddr *) &Remote_Address,
	 sizeof(Remote_Address)));


	printf("Connection successful\n");

	bzero(buffer, 1024);
	strcpy(buffer, argv[3]);

	while(1) {
		n = write(client_socket, buffer, sizeof(buffer));
		printf("Write status: %d\n", n);
		bzero(buffer, 1024);
		n = read(client_socket, buffer, sizeof(buffer));
		printf("Read status: %d\n", n);
		printf("Server msg: %s\n", buffer);
		fgets(buffer, sizeof(buffer), stdin);
	}
	
	exit(0);
}