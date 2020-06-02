#include "coordinator.h"
coordinator::coordinator(Socket co_,vector<Socket> pa_list)
{
    socketInfo=co_;
    this->pa_list=pa_list;//给同名变量赋值，this->pa_list指明是类中的pa_list   
    cout<<"create a coordinator "<<socketInfo.IP<<":"<<socketInfo.port<<endl;
    for(int i=0;i<pa_list.size();i++)
    {
        cout<<"painfo:"<<this->pa_list[i].IP<<":"<<this->pa_list[i].port<<endl;
        //状态表初始化：状态均为init 事务id为0
        TaskTableItem tmp={.TaskId=0,.TimeStamp=0,.TaskState=init};
        TaskTable.push_back(tmp);
    }
}
void* oneRequest(void* args)//void创建不了线程- -
{
    ThreadParas* para = (ThreadParas*) args;

    string msg=para->msg;
    
    Socket socketInfo=para->socketMSG;
    struct sockaddr_in serverAdd;

    bzero(&serverAdd, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = inet_addr(socketInfo.IP.c_str());
    serverAdd.sin_port = htons(socketInfo.port);
    
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
 
    int connResult = connect(connfd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (connResult < 0) 
    {
        cout << "connect error" << std::endl;
        para->state=false;
        para->returnMSG="-ERROR\r\n";
        close(connfd);
    }  
    else
    {
        Connect* conn = new Connect(connfd);
        if(!conn->sendLine(connfd,msg))
        {
            cout<<"send faild!";
            para->state=false;
            para->returnMSG="-ERROR\r\n";
        }
        string respone;
        if(conn->RecvLine(connfd,respone))//收客户端回复到请求。
        {
            //cout<<"get a response"<<endl;
            para->returnMSG=respone;
            para->state=true;
        }
        close(connfd);
    }
    
}
MSG coordinator::RequestToParticipant(string msg)//将请求发送给参与者,协调者负责主动连接参与者。
{
    int len=pa_list.size();
    //len=2;//debug用
    int connfd[len];//每与参与者建立一个请求，生成一个connfd.
    pthread_t req_th[len];//线程
    ThreadParas req_Para[len];//线程参数
    MSG returnMSG={.state=false,.message="-ERROR\r\n"};
    for(int i=0;i<len;i++)
    {
        ThreadParas req_args;
        req_Para[i].msg=msg;
        req_Para[i].socketMSG=pa_list[i];
        if(pthread_create(&req_th[i], NULL, oneRequest, &req_Para[i])!=0)//create thread
        {
            perror("pthread_create failed");
        }                     
    }
    for(int i=0;i<len;i++)
        pthread_join(req_th[i],NULL);
    for(int i=0;i<len;i++)
    {
        //cout<<"线程回复:"<<req_Para[i].msg<<endl;
        if(!req_Para[i].state)
        {
            //该线程已断开，更改该线程的状态表值。
            TaskTable[i].TaskState=zombie;
        }
        else
        {
            //cout<<"change participant TaskId from "<<TaskTable[i].TaskId<<" to "<<TaskId<<endl;
            TaskTable[i].TaskId=TaskId;
            returnMSG.state=true;
            returnMSG.message=req_Para[i].returnMSG;
        }     
    }
    co_state=READY;
    //cout<<"request finish!"<<endl;
    return returnMSG;
}//此函数需要统计多个线程的值是否正常，是否超时。

void coordinator::recvFromClient()
{
    int co_sock=SocketApi::Socket();
    SocketApi::Bind(co_sock,socketInfo.port,socketInfo.IP);
    SocketApi::Listen(co_sock);
    while(1)//多个客户端连接
	{
       std::string peer_ip;
       int peer_port;
       int sock = SocketApi::Accept(co_sock,peer_ip,peer_port);   
       //std::cout << "part_sock :" << sock <<std::endl;
       if(sock>= 0)//收到一个客户
       {
       	  //std::cout << peer_ip << " : " << peer_port <<std::endl;
          Connect* part_conn = new Connect(co_sock);
          string line;
          while(1)//客户端多个请求。
          {    
            part_conn->RecvLine(sock,line);
            cout<<"coordinator recv the request"<<endl;
            TaskId=TaskId+1;
            LogItem logitem={.state="REQUEST",.time="",.TaskId=this->TaskId,.massage=line};
            logwriter(logitem);
            //处理任务
            MSG msg=RequestToParticipant(line);
            if(msg.state)
            {
                LogItem logitem={.state="FINISH",.time="",.TaskId=this->TaskId,.massage=line};
                logwriter(logitem);
            }
            //发回给客户端。
            cout<<"send"<<msg.message<<"to client"<<endl;
            part_conn->sendLine(sock,msg.message);
          }
          
       }
	}
}
bool coordinator::logwriter(LogItem data)//向日志文件写入一行。
{
    data.time=getTime();
    log.push_back(data);
}
int HeartTask::Run()
{
    int connfd = GetConnFd();
    Connect* heart_conn = new Connect(connfd);
    string line;//接收到到字符串
    while(1)
    {
        heart_conn->RecvLine(connfd,line);
        cout<<"recv heart:"<<line<<endl;
    }        
}

void coordinator::recvHeart()
{
    int co_sock=SocketApi::Socket();
    SocketApi::Bind(co_sock,socketInfo.port,socketInfo.IP);
    SocketApi::Listen(co_sock);
    CThreadPool Pool(10);
    while(1)
	{
       std::string peer_ip;
       int peer_port;
       int sock = SocketApi::Accept(co_sock,peer_ip,peer_port);   
       //std::cout << "part_sock :" << sock <<std::endl;
       if(sock>= 0)
       {
       	  //std::cout << peer_ip << " : " << peer_port <<std::endl;
          HeartTask* ta=new HeartTask;
          ta->SetConnFd(sock);    
          ta->SetIP(peer_ip);
          ta->SetPort(peer_port);
          Pool.AddTask(ta);             
       }
	}
}