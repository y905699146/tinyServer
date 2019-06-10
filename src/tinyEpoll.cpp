#include<stdio.h>
#include "tinyEpoll.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
using namespace TinyServer;

int TinyEpoll::CreateSocket()
{
    this.Socketfd = socket( AF_INET, SOCK_STREAM, 0 );
    if( this.Socketfd < 0)  
    {  
        perror("socket error");  
        exit(-1);  
    } 
}
int TinyEpoll::Listen(int backlog = SOMAXCONN) const
{
    int ret = ::listen(this.Socketfd,backlog);
    if (ret<0)
    {
        perror("listen error");  
        exit(-1);  
    }
}

int TinyEpoll::Bind(const char* ip,unsigned short int port) const{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;  
    saddr.sin_port   = htons(port);  
    inet_pton(AF_INET,ip,&serv_addr.sin_addr); 
}
void Cal(int a,int b){
    printf("%d",a+b);
}