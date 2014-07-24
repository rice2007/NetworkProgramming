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
	int clientfd, sendErr, byteSize, optInt;
	int on = 1, off = 0, i = 0;
	unsigned int fromLength;
	struct hostend *hp;
	struct sockaddr_in clientAddr, fromAddr, serverAddr;

	if (argc != 3) {
		printf("Incorrect args: <server IP> <port>\n");
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
	serverAddr.sin_port = htons(atoi(argv[2]));
	optInt = setsockopt(clientfd, SOL_SOCKET, SO_BROADCAST, &on, 4);
	serverAddr.sin_addr.s_addr |= htonl(0x1ff);
	printf("bcast address: %s\n", inet_ntoa(serverAddr.sin_addr));
	optInt = setsockopt(clientfd, SOL_SOCKET, SO_BROADCAST, &off, 4);
	while (i < 10) {
		recvfrom(clientfd, buffer, 1024, 0, (struct  sockaddr*) &fromAddr,
			&fromLength);
		sleep(1);
		i++;	
	}
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
