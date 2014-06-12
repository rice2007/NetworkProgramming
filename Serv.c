#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd, connfd, readInt;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025], localBuffer[1024];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(10551);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);

        ticks = time(NULL);
        read(connfd, sendBuff, strlen(sendBuff));
        strcpy(localBuffer, sendBuff);
        printf("Client msg: %s\n", localBuffer);
        //printf("Client message: %s", sendBuff);
        write(connfd, sendBuff, strlen(sendBuff));

        //close(connfd);
        sleep(1);
     }
}
