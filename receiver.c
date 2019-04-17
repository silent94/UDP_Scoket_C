#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ev.h>

#define RECEIVER_PORT 8789

typedef struct receiver{
    ev_io server_io;
    int sock_fd;
}RECEIVER_IO;

int recv_total = 0;

void rcv_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
    if(EV_ERROR & revents){
        perror("got invalid event");
        return;
    }
    
    int recv_len = 0;
    int buflen = 1024;
    struct sockaddr_in sender;
    int s_len = sizeof(sender);
    char *buffer = (char *)calloc(buflen, sizeof(char));
    RECEIVER_IO *rptr = (RECEIVER_IO *) watcher;
    
    recv_len = recvfrom(rptr->sock_fd, buffer, buflen, 0, (struct sockaddr *)&sender, (socklen_t *)&s_len);

    if (recv_len > 0) {
        //printf("%s", buffer);
        //fflush(stdout);
        recv_total += recv_len;
    }
    
    printf("Total size of received file = %d\n", recv_total);
    free(buffer);
}

int main(void){
    struct sockaddr_in server;
    struct ev_loop *loop = ev_default_loop(0);
    RECEIVER_IO receiver;

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(RECEIVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if((receiver.sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("failed to open socket");
        abort();
    }
    int on=1;
    setsockopt(receiver.sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    if(bind(receiver.sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("bind failed");
        abort();
    }

    /* use to check recv buffer size
    int bufferSize;
    socklen_t socklen = sizeof(bufferSize);
    getsockopt(receiver.sock_fd, SOL_SOCKET, SO_RCVBUF, (void *)&bufferSize, &socklen);
    printf("Size of recv buffer is %d\n", bufferSize);
    */

    // use to set recv buffer size
    int nRecvBuf=1000*1024;         
    setsockopt(receiver.sock_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
    
    ev_io_init(&(receiver.server_io), rcv_cb, receiver.sock_fd, EV_READ);
    ev_io_start(loop, &(receiver.server_io));

    ev_loop(loop, 0);

    return 0;
}