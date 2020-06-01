#include "participant.h"
#include "KVserver.h"
#define MAXDATASIZE 100000
participant::participant(Socket pa_,Socket co_)
{
    socketInfo=pa_;
    CoInfo=co_;
    cout<<"create a participant "<<socketInfo.IP<<":"<<socketInfo.port<<endl;
    cout<<"coordinator "<<CoInfo.IP<<":"<<CoInfo.port<<endl;
    pa_state=WAIT;
    
}
MSG participant::recvFromCoorinator()
{
    int pa_sock=SocketApi::Socket();
    SocketApi::Bind(pa_sock,socketInfo.port,socketInfo.IP);
    SocketApi::Listen(pa_sock);
    while(1)
	{
       std::string peer_ip;
       int peer_port;
       int sock = SocketApi::Accept(pa_sock,peer_ip,peer_port);   
       std::cout << "part_sock :" << sock <<std::endl;
       if(sock>= 0)
       {
       	  std::cout << peer_ip << " : " << peer_port <<std::endl;
          Connect* part_conn = new Connect(pa_sock);
          string line;//接收到到字符串
          part_conn->RecvLine(sock,line);
          logwriter(to_string(TaskId)+" REQUEST "+line);//接到请求，写日志。
          MSG msgResult=MsgAnalyze(line);//对操作进行处理，得到处理结果。

          cout<<"participant recv and finish the request:"<<msgResult.message<<endl;
          logwriter("FINISH "+line);//完成任务，写日志。
        //   if(msgResult.state)//得到正确处理结果。
        //   {
        //       cout<<"participant recv and finish the request:"<<msgResult.message<<endl;
        //       logwriter("FINISH "+line);//完成任务，写日志。
        //   }
        //   else
        //   {
        //       cout<<"participant recv and ABORT the request:"<<line<<endl;
        //       logwriter("ABORT "+line);
        //   }
          part_conn->sendLine(sock,msgResult.message);
          
       }
	}
}
MSG participant::MsgAnalyze(string resp)
//处理resp
{
    MSG resultMSG={.state=false,.message="-ERROR\r\n"};//初始化
    vector<string> result=mysplit(resp,"\r\n");
    for(int i=0;i<result.size();i++)
    {
        cout<<result[i]<<endl;
    }
    if(result.size()<5) return resultMSG;//错误信息
    if(result[2]=="GET")
    {
        string key=result[4];
        return get(key);
    }
    else if(result[2]=="SET")
    {
        string key=result[4];
        string value="";
        for(int i=6;i<result.size();i+=2)
        {
            if(i!=6) value=value+" ";
            value=value+result[i];
        }
        if(value=="") return resultMSG;
        return set(key,value);
    }
    else if(result[2]=="DEL")
    {
        vector<string> keyList;
        for(int i=4;i<result.size();i+=2)
        {
            keyList.push_back(result[i]);
        }
        return delate(keyList);
    }
    else return resultMSG;


}
MSG participant::set(string key,string value)//对本地数据库进行set操作
{
    MSG resultMSG={.state=false,.message="-ERROR\r\n"};//初始化
    map<string,string>::iterator iter=database.find(key);
    if(iter!=database.end())//有存在的
    {
        iter->second=value;
        resultMSG.state=true;
        resultMSG.message="+OK\r\n";//成功。
        return resultMSG;
    }
    else
    {
        pair< std::map< string,string >::iterator,bool > ret;
        ret=database.insert(pair<string,string>(key,value));
        if(ret.second)
        {
            resultMSG.state=true;
            resultMSG.message="+OK\r\n";//成功。
            return resultMSG;
        }
    }   
    return errorMSG;
}
MSG participant::delate(vector<string> keyList)
//对本地数据库进行delate操作
{
    MSG resultMSG={.state=false,.message="-ERROR\r\n"};//初始化
    int delNum=0;
    int len=keyList.size();
    for(int i=0;i<len;i++)
    {
        map<string,string>::iterator iter=database.find(keyList[i]);
       if(iter!=database.end())//成功删除
       {
           delNum++;
           database.erase(iter);
       }
    }
    string msg=":"+to_string(delNum)+"\r\n";
    resultMSG.message=msg;
    resultMSG.state=true;
    return resultMSG;
}
MSG participant::get(string key)
//对本地数据进行get操作.get value of key
{
    MSG resultMSG={.state=false,.message="-ERROR\r\n"};//初始化

    map<string,string>::iterator iter=database.find(key);
    if(iter!=database.end())//get到了
    {
        string value=iter->second;
        vector<string> result=mysplit(value," ");
        string msg='*'+to_string(result.size());
        // for(int i=0;i<result.size();i++)
        // {
        //     cout<<result[i]<<endl;
        // }
        for(int i=0;i<result.size();i++)
        {
            int len=result[i].length();
            msg=msg+"\r\n$"+to_string(len)+"\r\n"+result[i];
        }
        msg=msg+"\r\n";
        resultMSG.message=msg;
        resultMSG.state=true;
    }
    return resultMSG;
}
bool participant::logwriter(string data)//向日志文件写入一行。
{
    log.push_back(data);
}
string RecvHeartTask::Getdata()
{
    return data;
}
void RecvHeartTask::Setdata(string data)
{
    this->data=data;
}
int RecvHeartTask::Run()
{
    int heart_sock=GetConnFd();
    string Heart_msg=Getdata();
    Connect* conn = new Connect(heart_sock);
    while(1)
    {
        if(!conn->sendLine(heart_sock,Heart_msg))
        {
            cout<<"send faild!";
        }
        usleep(500000);
    }
}
void participant::heart()
{
    int heart_sock=SocketApi::Connect_sock(CoInfo.IP,CoInfo.port);//把创建socket的过程也放里面了。返回创建的用于connect的socket
    string Heart_msg=to_string(TaskId);
    Connect* conn = new Connect(heart_sock);
    while(1)
    {
        if(!conn->sendLine(heart_sock,Heart_msg))
        {
            cout<<"send faild!";
        }
        usleep(500000);
    }
    // CThreadPool Pool(2);
    // RecvHeartTask* ta=new RecvHeartTask;
    // ta->SetConnFd(heart_sock);
    // ta->Setdata(to_string(TaskId));
    // Pool.AddTask(ta);
}
bool participant::recovery()
{

}
