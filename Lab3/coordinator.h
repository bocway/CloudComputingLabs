#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "KVserver.h"
#include "protocolutil.h"
class coordinator
{
    private:
        Socket socketInfo;
        vector<Socket> pa_list;
        STATE co_state;
    public:
        coordinator(Socket co_,vector<Socket> pa_list);//创建协调者，并与多个参与者建立连接。
        MSG RequestToParticipant(string msg);//将请求发送给参与者
        void recvFromClient();
        void heart();//心跳
};

#endif