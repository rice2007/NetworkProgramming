#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

main(int argc, char* argv[]) {
	char buf[512];
	int client_socket;
	struct sockaddr_in localAddr_Active, Remote_Address;
	struct hostent *hp, *gethostbyname();
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0) {
		printf("errno = %d\n", errno); //Print error message
		perror("socket")l
	}
	bzero (&Remote_Address, sizeOf(Remote_Address))
	Remote_Address.sin_family = AF_Net;
	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		printf()//errno)
		perror("hostname");
	}
	Remote_Address.sin_port = htons(4096);
}
