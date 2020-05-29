#ifndef KVSERVER_H
#define KVSERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <pthread.h>

#include <assert.h>
#include <stdint.h>

#include <sys/stat.h>
#include <iostream>
//STL
#include <cstring>
#include <map>
#include <vector>
//socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <pthread.h>
#include <queue>
using namespace std;

struct MSG{
    bool state;
    string message;
};
const MSG errorMSG={.state=false,.message="-ERROR\r\n"};
struct Socket{
    string IP;
    int port;
};
enum STATE {WAIT,READY,DONE,RECOVERY};
vector<string> mysplit(string str,string pattern);

#endif