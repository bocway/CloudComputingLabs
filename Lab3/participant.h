#ifndef PARTICIPANT_H
#define PARTICIPANT_H
#include "KVserver.h"
#include<map>
class participant
{
    private:
        string ip;
        string port;
    public:
        participant(string ip,string port);//create participant with ip and port
        void set();
        void delate();

};

#endif