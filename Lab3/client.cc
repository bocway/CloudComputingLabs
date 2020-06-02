#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sstream>
#include<iostream>
#include<vector>
#define MAXLINE 4096
using namespace std;

//字符串分割函数
vector<string> mysplit(string str,string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
 
    for(int i=0; i<size; i++)
    {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             string s=str.substr(i,pos-i);
             result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}
int main(int argc, char** argv)
{
    int   sockfd, n;
    char  recvline[4096], sendline[4096], sendline1[4096];
    struct sockaddr_in  servaddr;
    string msg;


    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    }
    bool bDontLinger = false;

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&bDontLinger,sizeof(bool));
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8001);
    if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n",argv[1]);
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    }
    stringstream sstr;
        //sstr.clear()

    //for(int i = 0; i < 3; i++)
    printf("send msg to server: \n");
    while(1)
    {
        getline(cin,msg);
        sstr.clear();
        vector<string> words=mysplit(msg," "); 
        int len = 0;
        for (vector<string>::const_iterator iter = words.begin();iter != words.end(); iter++) 
            len++;
        char len_char = len + '0';
        string msg1 = "*";
        msg1 += len_char;
        if(words[0]=="SET")
        {
            msg1 += "\r\n$3\r\nSET\r\n$";
            len = words[1].length();
            len_char = len + '0';
            msg1 += len_char;
            msg1 += "\r\n";
            msg1 += words[1];
            msg1 += "\r\n";
            string using1 = "\"";
            vector<string> words1=mysplit(msg,using1); 
            vector<string> words2=mysplit(words1[1]," "); 
            for (vector<string>::const_iterator iter = words2.begin();iter != words2.end(); iter++) 
            {
                msg1 += "$";
                len = (*iter).length();
                len_char = len + '0';
                msg1 += len_char;
                msg1 += "\r\n";
                msg1 += (*iter);
                msg1 += "\r\n";
            }
        }
        else if(words[0]=="GET")
        {
            msg1 += "\r\n$3\r\nGET\r\n$";
            len = words[1].length();
            len_char = len + '0';
            msg1 += len_char;
            msg1 += "\r\n";
            msg1 += words[1];
            msg1 += "\r\n";
        }
        else if(words[0]=="DEL")
        {
            msg1 += "\r\n";
            len = words[1].length();
            len_char = len + '0';
            string using1 = "\"";
            for (vector<string>::const_iterator iter = words.begin();iter != words.end(); iter++) 
            {
                msg1 += "$";
                len = (*iter).length();
                len_char = len + '0';
                msg1 += len_char;
                msg1 += "\r\n";
                msg1 += (*iter);
                msg1 += "\r\n";
            }
        }
        cout<<msg1<<endl;
        sstr<<msg1;
        sstr>>sendline1;
        //strcpy(sendline,"*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n");
        strcpy(sendline,sendline1);
        ssize_t s = send(sockfd,msg1.c_str(),msg1.size(),0);
        // if( SendAll(sockfd, msg1) < 0){
        //     printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        // }
        // if( send(sockfd, sendline, strlen(sendline), 0) < 0){
        //     printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        // }    
        printf("send msg to server: \n");
    }
    close(sockfd);
    return 0;
}