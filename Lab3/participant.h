#ifndef PARTICIPANT_H
#define PARTICIPANT_H
#include "KVserver.h"
#include "protocolutil.h"
class participant
{
    private:
        Socket socketInfo;//保存该节点的端口号和IP
        Socket CoInfo;//保存协调者的端口号和IP
        vector<LogItem> log;//本地日志文件
        STATE pa_state;//当前节点状态
        int TaskId;//当前执行的TaskID

        map<string,string> database;//本地数据库
        MSG set(string key,string value);//对本地数据库进行set操作
        MSG delate(vector<string> keyList);//对本地数据库进行delate操作
        MSG get(string key);//对本地数据进行get操作.
    public:
        participant(Socket pa_,Socket co_);
        MSG recvFromCoorinator();//与协调者建立连接，执行通信过程
        MSG MsgAnalyze(string resp);//解析协调者发送的命令
        bool logwriter(LogItem data);//写入日志
        void heart();//向协调者发送心跳。
        bool recovery(vector<LogItem> log);//恢复状态。
};

#endif