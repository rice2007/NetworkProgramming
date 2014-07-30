#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	char buffer[1024];
	int bindErr, byteSize, sendErr, serverfd;
	unsigned int fromLength;
	struct sockaddr_in fromAddr, serverAddr;

	serverfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverfd < 0) {
		perror("socket");
		exit(-1);
	}
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_port=htons(10500);
	bindErr = bind(serverfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	if (bindErr < 0) {
		perror("bind");
		exit(-1);
	}

	while(1) {
		fromLength = sizeof(fromAddr);
		byteSize = recvfrom(serverfd, buffer, 1024, 0, (struct sockaddr *) &fromAddr,
			&fromLength);
		if (byteSize < 0) {
			perror("recvfrom");
		} else if (byteSize == 0) {
			printf("Client %d has disconnected.", serverAddr.sin_addr.s_addr);
		} else {
			printf("%d bytes from IP %s: %s", byteSize, inet_ntoa(serverAddr.sin_addr), buffer);
			if (!strcmp(buffer, "end")) {
				printf("Shutdown cmd rec'd.");
				exit(0);
			}
		}

		strcpy(buffer, "msg rec'd");
		sendErr = sendto(serverfd, buffer, 1024, 0, (struct sockaddr *) &fromAddr, fromLength);
		if (sendErr < 0) {
			perror("sendto");
		}
	}
}
