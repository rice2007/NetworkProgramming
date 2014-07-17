/*


int main(int argc, char* argv[]) {

	char buffer[1024];
	int clientSocket, byteSize;
	unsigned int fromLength;
	struct hostend *hp;
	struct sockaddr_in clientAddr, fromAddr, serverAddr;

	clientSocket =  socket(AF_INET, SOCK_DGRAM, 0);
	hp = gethostbyname(argv[1]);
	server.sin_family = AF_INET;
	//Fix
	memcpy();

	server.sin_port=htons(4096);

	byteSize = sendto(clientSocket, argv[2], strlen(argv[2]) + 1, 0,
		(struct sockaddr*) &serverAddr, sizeof(serverAddr);
	fromLength = sizeof(fromAddr);
	byteSize = recvfrom(clientSocket, buffer, 1024, 0, (struct  sockaddr*) &fromAddr,
		&fromLength);
	printf("%d bytes from IP %s (%s)\n", byteSize, inet_ntoa(from.sin_addr), buffer);
	close(clientSocket);
	return 1;
}*/
