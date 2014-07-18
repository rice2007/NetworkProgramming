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

int main(int argc, char* argv[]) {

	char buffer[1024];
	int clientfd, sendErr, byteSize;
	unsigned int fromLength;
	struct hostend *hp;
	struct sockaddr_in clientAddr, fromAddr, serverAddr;

	if (argc != 2) {
		printf("Incorrect args: <server IP> <port>");
		exit(-1);
	}
	clientfd =  socket(AF_INET, SOCK_DGRAM, 0);
	hp = gethostbyname(argv[1]);
	if (hp == NULL) {
		perror("gethostbyname");
		exit(-1);
	}
	if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) == 0) {
		printf("Invalid network address\n");
		exit(-1);
	} else if ((inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) < 0)) {
		perror("inet_pton");
		exit(-1);
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));
	//memcpy();

	while (1) {
		fgets(buffer, sizeof(buffer), stdin);
		sendErr = sendto(clientfd, argv[1], strlen(argv[1]) + 1, 0,
			(struct sockaddr*) &serverAddr, sizeof(serverAddr));
		if (sendErr < 0) {
			perror("sendto");
		}
		fromLength = sizeof(fromAddr);
		byteSize = recvfrom(clientfd, buffer, 1024, 0, (struct  sockaddr*) &fromAddr,
			&fromLength);
/*		printf("%d bytes from IP %s (%s)\n", byteSize, inet_ntoa(from.sin_addr), buffer);
		close(clientfd);*/
	}
	return 1;
}
