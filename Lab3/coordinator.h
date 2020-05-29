#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "KVserver.h"
#include "protocolutil.h"
class coordinator
{
    private:
        Socket socketInfo;
        vector<Socket> pa_list;
        vector<TaskTableItem> TaskTable;//刘总所需要的状态表。
        STATE co_state;
        int TaskId;
    public:
        coordinator(Socket co_,vector<Socket> pa_list);//创建协调者，并与多个参与者建立连接。
        MSG RequestToParticipant(string msg);//将请求发送给参与者
        void recvFromClient();
        void recvHeart();//接收心跳
};

#endif