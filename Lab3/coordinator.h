#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "KVserver.h"
#include "protocolutil.h"
class coordinator
{
    private:
        Socket socketInfo;//保存该协调者的IP与端口。
        vector<Socket> pa_list;//保存各个参与者的IP与端口。
        vector<TaskTableItem> TaskTable;//状态表。记录各参与者的状态。
        vector<LogItem> log;//协调者日志文件。
        STATE co_state;//协调者状态。
        int TaskId;//当前事务ID
    public:
        coordinator(Socket co_,vector<Socket> pa_list);//创建协调者
        MSG RequestToParticipant(string msg);//将请求发送给参与者
        void recvFromClient();//接收客户端请求
        void recvHeart();//接收参与者心跳
        bool logwriter(LogItem data);//写日志
};

#endif