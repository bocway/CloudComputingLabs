#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "KVserver.h"
class coordinator
{
    private:
        string ip;
        string port;
    public:
        coordinator(string ip,string port);//create coordinator with ip and port
};

#endif