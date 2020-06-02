#ifndef KVSERVER_H
#define KVSERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
//STL
#include <cstring>
#include <map>
#include <vector>
#include <queue>
//socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
//mutex and thread
#include <pthread.h>
#include <semaphore.h> 
#include "Thread.h"

using namespace std;
enum STATE {init,live,zombie,recovery,WAIT,READY,DONE,RECOVERY};
struct MSG{
    bool state;
    string message;
//state代表连接状态，false为连接失败。
//message为要传输的信息。    
};
const MSG errorMSG={.state=false,.message="-ERROR\r\n"};
struct Socket{
    string IP;
    int port;
};
struct LogItem{
    string state;//任务状态(COMMIT,ABORT)
    string time;//当前时间
    int TaskId;//任务号
    string massage;//任务命令
};
struct TaskTableItem{
    Socket pa_info;//参与者信息
    int TaskId;//参与者目前执行的任务号
    int64_t TimeStamp;//发送时刻
    STATE State;//参与者状态
};
typedef struct {
   int socket;
   string msg;
   Socket socketMSG;
   bool state;
   string returnMSG;
} ThreadParas;

vector<string> mysplit(string str,string pattern);
string getTime();

#endif