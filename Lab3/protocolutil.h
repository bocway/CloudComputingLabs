#ifndef __PROTOCOLUTIL_HPP__
#define __PROTOCOLUTIL_HPP__

#include<iostream>
#include<unordered_map>
#include<iostream>
#include<sstream>
#include<arpa/inet.h>
#include<string>
#include<vector>
#include<sys/stat.h>
#include<unistd.h>
#include<algorithm>
#include<fcntl.h>
#include<errno.h>
#include<fstream> 
#include<map>
#include "write_log.hpp"
#define NORMAL 0
#define WARNING 1
#define ERROR 2

#define BACKLOG 5
#define BUFF_NUM 1024
#define MAX_PARTICIPANT 5
//#define sec 4
extern const char* ErrLevel[];


class SocketApi
{
    public:
    	static int Socket();
    	static void Bind(int sock,int port);
    	static void Listen(int sock);
    	static int Accept(int listen_sock,std::string& ip,int &port);
    	static int Connect_sock(std::string SERV_ADDR,int SERV_PORT);	
};

class Connect
{
    private:
        int sock;
        std::string state;
    public:
        Connect(int sock_) : sock(sock_){};
        void setState(std::string state_);
        std::string getState();
        bool sendLine(int connfd,std::string line);
        bool RecvLine(int sock_,std::string& line);
};



#endif
