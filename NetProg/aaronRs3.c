
int main(int argc, char* argv[]) {

	char buffer[1024];
	int byteSize, serverSocket;
	int on = 1, off = 0;
	unsigned int fromLength;
	struct hostend *hp;
	struct sockaddr_in slint, fromAddr, serverAddr;

	clientSocket =  socket(AF_INET, SOCK_DGRAM, 0);
	hp = gethostbyname(argv[1]);
	server.sin_family = AF_INET;
	//Fix
	memcpy();

	server.sin_port=htons(4096);

	while(1) {
		printf();
		byteSize = recvfrom(serverSocket, buffer, 1024, 0, (struct sockaddr*) &fromAddr,
			&fromLength);
		printf();
		if (!strcmp(buffer, "end")) {
			break;
		}
		strcpy(buffer, "msg rec'd");
	}
}