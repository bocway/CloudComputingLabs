#include <assert.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/time.h>

#include <fstream>
#include <getopt.h>

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
static struct option long_options[] =
    {  
        {"config_path", 1,NULL, 'r'},
        {"optarg", 1,NULL, 'o'},
        {"noarg",  1,         NULL,'n'},
        {NULL,     0,                      NULL, 0},
}; 
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
  int opt;
    int digit_optind = 0;
    int option_index = 0;
    char *string2 = "a::b:c:d";
    
    while((opt =getopt_long_only(argc,argv,string2,long_options,&option_index))!= -1)
    {  
        //printf("optarg = %s",optarg);
      string file_name = optarg;
      ifstream infile; 
      infile.open(file_name.data());   //将文件流对象与文件连接起来 
      assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

      string s;
      while(getline(infile,s))
      {
        if(s[0]=='!')
          continue;
        else if(s[0]=='m')
        {
          int location = 0;
          char flag = '0';
          for(int i=0;i<s.length();i++)
          {
            if(s[i]==' ')
              flag = 1;
            else if(flag==1)
              {flag = s[i];break;}
          }
          if(flag == 'h')
            //system("./helloworld");    
            cout<<"run!";     
        }        
        //system("./helloworld");
              //cout<<s<<endl;
        cout<<"run!";   
      }
      infile.close();             //关闭文件输入
    }
  if(DEBUG)
  {
      printf("--------------------\n"); 
      string str=a.MsgAnalyze("*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n").message;  
      string str2=a.MsgAnalyze("*2\r\n$3\r\nGET\r\n$7\r\nCS06142\r\n").message;
      cout<<str<<endl<<str2;
  }
}

