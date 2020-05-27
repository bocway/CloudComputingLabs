#ifndef PARTICIPANT_H
#define PARTICIPANT_H
#include "KVserver.h"

class participant
{
    private:
        Socket socketInfo;
        map<string,string> database;//本地数据库
        MSG set(string key,string value);//对本地数据库进行set操作
        MSG delate(vector<string> keyList);//对本地数据库进行delate操作
        MSG get(string key);//对本地数据进行get操作.get value of key
    public:
        participant(Socket pa_);//create participant with ip and port

        MSG MsgAnalyze(string resp);//解析协调者发送的命令

};

#endif