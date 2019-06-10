#include <sys/types.h>
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
#include "tinyEpoll.h"


//using namespace TinyServer;
#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024

int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

int main(int argc,char *argv[])  
{  
    const char* ip= argv[1];;
    const char* port= argv[2];
    printf(ip,port);
    TinyServer::TinyEpoll* tinyServer =new TinyServer::TinyEpoll();
    int ret = tinyServer->CreateSocket();
    if (ret == -1) {
        return ret;
    }
    ret = tinyServer->Bind(ip,12345);
    if (ret == -1) {
        return ret;
    }
    ret = tinyServer->Listen(5);
    if (ret == -1) {
        return ret;
    }
    struct epoll_event events[ MAX_EVENT_NUMBER ];  
    int epfd = epoll_create(5); // 创建一个 epoll 的句柄，参数要大于 0， 没有太大意义    
    if( -1 == epfd ){    
        perror ("epoll_create");    
        return -1;    
    }       
  /*  struct epoll_event event;   // 告诉内核要监听什么事件    
    event.data.fd = 0;     //监听套接字  
    event.events = EPOLLIN; // 表示对应的文件描述符可以读  
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event);  
    if(-1 == ret){  
        perror("epoll_ctl");  
        return -1;  
    }    */
    addfd(epfd,tinyServer->GetScoketfd());

    while(1)  
    {     
        // 监视并等待多个文件（标准输入，udp套接字）描述符的属性变化（是否可读）    
        // 没有属性变化，这个函数会阻塞，直到有变化才往下执行，这里没有设置超时     
        ret = epoll_wait(epfd, events, MAX_EVENT_NUMBER, -1);    
          
        if(ret == -1){ // 出错    
            close(epfd);    
            perror("epoll");    
        }  
        else if(ret > 0)
        { // 准备就绪的文件描述符  

            for(int i=0;i<ret;i++)
            {
                if(!(events[i].events & EPOLLIN))
                {
                    continue;
                } 
                int sockfd = events[i].data.fd;
                if ( sockfd == tinyServer->GetScoketfd())
                {
                    struct sockaddr_in client_address;
                    socklen_t client_addrlength = sizeof( client_address );
                    int connfd = tinyServer->Accept();
                    addfd( epfd, connfd );
                }
                else
                {
                    if(events[i].events & EPOLLIN)
                    {
                        const char *buf[10240];
                        ssize_t s = read(sockfd, buf, sizeof(buf)-1);
                        if(s>0)
                        {
                            printf("client : %s\n", buf);
                            
                        }
                    }
                }
            }

        }
        else if(0 == ret){ // 超时    
            printf("time out\n");    
        } 
    }      
      
    return 0;  
} 