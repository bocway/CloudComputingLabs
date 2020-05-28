#include "participant.h"
#define MAXDATASIZE 100000
participant::participant(Socket pa_,Socket co_)
{
    socketInfo=pa_;
    CoInfo=co_;
    cout<<"create a participant "<<socketInfo.IP<<":"<<socketInfo.port<<endl;
    cout<<"coordinator "<<CoInfo.IP<<":"<<CoInfo.port<<endl;
}
MSG participant::MsgAnalyze(string resp)
//处理resp
{
    MSG resultMSG={.state=false,.message="-ERROR\r\n"};//初始化
    vector<string> result=mysplit(resp,"\r\n");
    // for(int i=0;i<result.size();i++)
    // {
    //     cout<<result[i]<<endl;
    // }
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
    pair< std::map< string,string >::iterator,bool > ret;
    ret=database.insert(pair<string,string>(key,value));
    if(ret.second)
    {
        resultMSG.state=true;
        resultMSG.message="+OK\r\n";//成功。
        return resultMSG;
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
       }
    }
    string msg=":"+to_string(delNum)+"\r\n";
    resultMSG.message=msg;
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
    }
    return resultMSG;
}
bool participant::logwriter(string data)
{
    this->log.push_back(data);
    return true;
}
// void recmessage(int sockfd){
// 	while(1){
// 		int numbytes;
// 		char buf[MAXDATASIZE];
		
// 		if((numbytes = recv(sockfd,buf,MAXDATASIZE,0))==-1){
// 			perror("recv");
// 			exit(1);
// 		}
// 		buf[numbytes]='\0';
// 		if(strcmp(buf,"exit")==0){
// 			printf("Server is closed\n");
// 			close(sockfd);
// 			exit(1);
// 		}              
// 		printf("Server:%s\n",buf);
// 	}/*while*/
// }
// MSG participant::recvFromCoorinator()
// {
//     int so=sockfd;
//     if((pthread_create(&recthread,NULL,(void*)recmessage,sockfd))!= 0){
// 		printf("create thread error!\r\n");
// 		exit(1);
// 	}
// }