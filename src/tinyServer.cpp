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
#include "tinyEpoll.h"

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
    int ret = 0;  
    struct sockaddr_in saddr;   
    bzero(&saddr,sizeof(saddr));  
    saddr.sin_family = AF_INET;  
    saddr.sin_port   = htons(8000);  
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);  
      
    //创建套接字  
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    if( listenfd < 0)  
    {  
        perror("socket error");  
        exit(-1);  
    }  
      
    //套接字端口绑字  
    if(bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)  
    {  
        perror("bind error");  
        close(listenfd);         
        exit(-1);  
    }  
  
   // printf("input: \"sayto 192.168.220.X\" to sendmsg to somebody\033[32m\n");    
    struct epoll_event event;   // 告诉内核要监听什么事件    
    struct epoll_event wait_event;    
    struct epoll_event events[ MAX_EVENT_NUMBER ];  
    int epfd = epoll_create(10); // 创建一个 epoll 的句柄，参数要大于 0， 没有太大意义    
    if( -1 == epfd ){    
        perror ("epoll_create");    
        return -1;    
    }    
        
    event.data.fd = 0;     // 标准输入    
    event.events = EPOLLIN; // 表示对应的文件描述符可以读   
      
      
    // 事件注册函数，将标准输入描述符 0 加入监听事件    
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event);    
    if(-1 == ret){    
        perror("epoll_ctl");    
        return -1;    
    }    
        
    event.data.fd = udpfd;     // 有名管道    
    event.events = EPOLLIN; // 表示对应的文件描述符可以读    
        
    // 事件注册函数，将有udp描述符udpfd 加入监听事件    
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, udpfd, &event);    
    if(-1 == ret){    
        perror("epoll_ctl");    
        return -1;    
    }    
      
    while(1)  
    {     
        // 监视并等待多个文件（标准输入，udp套接字）描述符的属性变化（是否可读）    
        // 没有属性变化，这个函数会阻塞，直到有变化才往下执行，这里没有设置超时     
        ret = epoll_wait(epfd, &wait_event, 2, -1);    
          
        write(1,"UdpQQ:",6);  
          
        if(ret == -1){ // 出错    
            close(epfd);    
            perror("epoll");    
        }  
        else if(ret > 0){ // 准备就绪的文件描述符    
            char buf[100] = {0};    
            if( ( 0 == wait_event.data.fd )     
            && ( EPOLLIN == wait_event.events & EPOLLIN ) ){ // 标准输入    
                  
                fgets(buf, sizeof(buf), stdin);  
                buf[strlen(buf) - 1] = '\0';  
                if(strncmp(buf, "sayto", 5) == 0)  
                {  
                    char ipbuf[16] = "";  
                    inet_pton(AF_INET, buf+6, &caddr.sin_addr);//给addr套接字地址再赋值.  
                    printf("\rsay to %s\n",inet_ntop(AF_INET,&caddr.sin_addr,ipbuf,sizeof(ipbuf)));  
                    continue;  
                }  
                else if(strcmp(buf, "exit")==0)  
                {  
                    close(udpfd);  
                    exit(0);  
                }  
                sendto(udpfd, buf, strlen(buf),0,(struct sockaddr*)&caddr, sizeof(caddr));    
                    
            }  
            else if( ( udpfd == wait_event.data.fd )     
            && ( EPOLLIN == wait_event.events & EPOLLIN )){ //udp套接字    
                struct sockaddr_in addr;  
                char ipbuf[INET_ADDRSTRLEN] = "";  
                socklen_t addrlen = sizeof(addr);  
                  
                bzero(&addr,sizeof(addr));  
                  
                recvfrom(udpfd, buf, 100, 0, (struct sockaddr*)&addr, &addrlen);  
                printf("\r\033[31m[%s]:\033[32m%s\n",inet_ntop(AF_INET,&addr.sin_addr,ipbuf,sizeof(ipbuf)),buf);    
            }    
                
        }  
        else if(0 == ret){ // 超时    
            printf("time out\n");    
        }    
    }  
      
    return 0;  
} 