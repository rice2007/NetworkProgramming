#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc char **argv) {

	char buffer[4096];
	int client_socket;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	bzero (&Remote_Address, sizeOf(Remote_Address));
	Remote_Address.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	Remote_Address.sin_port = htons(4096);
	connect(client_socket, (struct sockadder *) &Remote_Address, sizeOf(Remote_Address) < 0);
	write(client_socket, argv[2], strlen(argv[2]) + 1);
	read (char, buffer, 512);
	printf("CLIENT: message from server: %s \n", buffer);
	close(client_socket);
	printf("CLIENT: exit \n");
	exit(0);
}