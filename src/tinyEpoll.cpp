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
    Socketfd = ::socket( AF_INET, SOCK_STREAM, 0 );
    if( this->Socketfd < 0)  
    {  
        perror("socket error");  
        exit(-1);  
    } 
    /*
        设置为 1,0表示套接字关闭时，TCP丢弃保留在发送缓存区的数据并发送给RST给对方
    */
    struct linger tmp={1,0};
    setsocketopt(Socketfd,SOL_SOCKET,SO_LINGER,&tmp,sizeof(tmp));
}
int TinyEpoll::Listen(int backlog)
{
    int ret = ::listen(Socketfd,backlog);
    if (ret<0)
    {
        perror("listen error");  
        exit(-1);  
    }
    return 0;
}

int TinyEpoll::Bind(const char* ip,unsigned short int port) const{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;  
    saddr.sin_port   = htons(port);  
    inet_pton(AF_INET,ip,&saddr.sin_addr); 
    int ret = ::bind(Socketfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret<0)
    {
        perror("bind error");  
        exit(-1); 
    }
    return 0;
}

int TinyEpoll::GetScoketfd() const{
    return Socketfd;
}

int TinyEpoll::Accept() const{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof( client_address );
    int connfd = ::accept(Socketfd,( struct sockaddr* )&client_address,&client_addrlength);
    if(connfd<0)
    {
        perror("accept err \n");
    }
    return connfd;
}