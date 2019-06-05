#pragma once
class TinyEpoll{
    public:
        int epfd;
        struct epoll_event *events;
};

void Cal(int a,int b);
