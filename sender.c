#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define SENDER_PORT 8786
#define RECEIVER_PORT 8789 

int main(void){
    const int buflen = 1024;
    char *array = (char *)malloc(buflen);
    FILE *txtptr;
    int sockfd;
    struct sockaddr_in sender, receiver;
    memset((char *) &sender, 0, sizeof(sender));
    memset((char *) &receiver, 0, sizeof(receiver));

    if((txtptr = fopen("./txtfile.txt","r")) == NULL){
        perror("File open error");
        abort();
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("failed to open socket");
        abort();
    }

    sender.sin_family = AF_INET;
    sender.sin_port = htons(SENDER_PORT);
    sender.sin_addr.s_addr = htonl(INADDR_ANY);

    int on=1;
    int nRecvBuf=1000*1024;         
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&nRecvBuf, sizeof(int));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    
    if(bind(sockfd, (struct sockaddr *)&sender, sizeof(sender)) == -1){
        perror("bind failed");
        abort();
    }

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(RECEIVER_PORT);
    inet_aton("127.0.0.1", &(receiver.sin_addr));

    int nread = 0;
    while((nread = fread(array, 1, buflen, txtptr)) > 0){
        while(sendto(sockfd, array, nread, 0, (struct sockaddr *)&receiver, sizeof(receiver)) == -1){
            printf("failed to send packet : %s\n", strerror(errno));
            sleep(1);
        }
        memset(array, 0, buflen);
        usleep(100);
    }
    printf("End of the file ! Entire file has been sent !\n");
    printf("The size of file is %ld \n", ftell(txtptr));

    free(array);
    close(sockfd);
    fclose(txtptr);
    return 0;
}