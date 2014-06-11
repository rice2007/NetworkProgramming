#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

	char buffer[1024];
	int listenfd, connfd;
	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(4096);
	serv_addr.sin_port = htons(5000);
	bind(listenfd, )
	listen(listenfd, 5);

	while(1) {
		//connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		write();
	}
}
