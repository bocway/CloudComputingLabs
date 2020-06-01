#ifndef KVSERVER_H
#define KVSERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include <pthread.h>

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
struct MSG{//state代表连接状态，false为连接失败。message为要传输的信息。
    bool state;
    string message;
};
const MSG errorMSG={.state=false,.message="-ERROR\r\n"};
struct Socket{
    string IP;
    int port;
};
struct TaskTableItem{
    int TaskId;
    int64_t TimeStamp;
    STATE TaskState;
};
typedef struct {
   int socket;
   string msg;
   bool state;
   string returnMSG;
} ThreadParas;

vector<string> mysplit(string str,string pattern);
int64_t now();

#endif