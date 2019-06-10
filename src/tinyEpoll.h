#pragma once

namespace TinyServer{

    class TinyEpoll{
    public:
        int epfd;
        struct epoll_event *events;
    public:
        explicit TinyEpoll():Socketfd(0){};
        int CreateSocket() ;
        int Bind(const char* ip,unsigned short int port) const;
        int Listen(int backlog = MaxConn) const;
        int Accept() const;
        int Connect();
        bool isValid() const; 
    private:
        int Socketfd;

    };
}


void Cal(int a,int b);
