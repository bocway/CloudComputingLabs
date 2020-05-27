#include <assert.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/time.h>

#include "KVserver.h"
#include "coordinator.h"
#include "participant.h"
#define DEBUG 1

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

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

int main(int argc, char* argv[])
{
  //詹总:获取参数并解析，构建相应的对象。需要完成两个构造函数的实现。

  //若协调者:
  Socket co_;
  vector<Socket> pa_list; 
  coordinator b(co_,pa_list);

  //若参与者:
  Socket pa_;
  participant a(pa_);
  
  if(DEBUG)
  {
      printf("--------------------\n"); 
      string str=a.MsgAnalyze("*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n").message;  
      string str2=a.MsgAnalyze("*2\r\n$3\r\nGET\r\n$7\r\nCS06142\r\n").message;
      cout<<str<<endl<<str2;

  }
}

