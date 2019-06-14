#pragma once

namespace TinyServer{
    #define SOMAXCONN 128
    class TinyEpoll{
    public:
        int epfd;
        struct epoll_event *events;
    public:
        explicit TinyEpoll():Socketfd(0){};
        int CreateSocket() ;
        int Bind(const char* ip,unsigned short int port) const;
        int Listen(int backlog = SOMAXCONN);
        int Accept() const;
        int Connect();
        int GetScoketfd() const;
        bool isValid() const; 
    private:
        int Socketfd;

    };

    class  
    {
    private:
        /* data */
    public:
         (/* args */);
        ~ ();
    };
    
     :: (/* args */)
    {
    }
    
     ::~ ()
    {
    }
    
}

