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
	int byteSize, sendErr, serverfd;
	int on = 1, off = 0;
	unsigned int fromLength;
	struct sockaddr_in slint, fromAddr, serverAddr;

	serverfd =  socket(AF_INET, SOCK_DGRAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_port=htons(10500);
	bind(serverfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	//memcpy();

	while(1) {
		byteSize = recvfrom(serverfd, buffer, 1024, 0, (struct sockaddr *) &fromAddr,
			&fromLength);
		if (byteSize < 0) {
			perror("recvfrom");
		} else if (byteSize == 0) {
			printf("Client %d has disconnected.", serverAddr.sin_addr.s_addr);
		}
		printf("%d bytes from IP %s (%s)\n", byteSize, inet_ntoa(serverAddr.sin_addr), buffer);
		printf("Rec'd msg: %s\n", buffer);
		if (!strcmp(buffer, "end")) {
			printf("Shutdown cmd rec'd.");
			exit(0);
		}
		strcpy(buffer, "msg rec'd");
		sendErr = sendto(serverfd, buffer, 1024, 0, (struct sockaddr *) &fromAddr, fromLength);
		if (sendErr < 0) {
			perror("sendto");
		}
	}
}