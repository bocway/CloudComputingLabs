#ifndef PARTICIPANT_H
#define PARTICIPANT_H
#include "KVserver.h"
#include "protocolutil.h"
class participant
{
    private:
        Socket socketInfo;
        Socket CoInfo;
        vector<string> log;
        STATE pa_state;
        int TaskId;
        pthread_t recthread;

        map<string,string> database;//本地数据库
        MSG set(string key,string value);//对本地数据库进行set操作
        MSG delate(vector<string> keyList);//对本地数据库进行delate操作
        MSG get(string key);//对本地数据进行get操作.get value of key
    public:
        participant(Socket pa_,Socket co_);//create participant with ip and port
        MSG recvFromCoorinator();
        MSG MsgAnalyze(string resp);//解析协调者发送的命令
        bool logwriter(string data);
        void heart();//向协调者发送心跳，待实现。
        bool recovery();//恢复状态，待实现。

};

#endif