#pragma once

#include<iostream>

class Message
{
    public:
        Message(){}
        ~Message(){}
        void send(const char* msg) noexcept{}
        void recv(const char* msg) noexcept{}
    private:
        const char* msg;
}