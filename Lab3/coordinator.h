#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "KVserver.h"
class coordinator
{
    private:
        Socket socketInfo;
        vector<Socket> pa_list;
    public:
        coordinator(Socket co_,vector<Socket> pa_list);//创建协调者，并与多个参与者建立连接。
        void RequestToParticipant();//将请求发送给参与者
};

#endif