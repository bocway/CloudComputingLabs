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
    int socket=para->socket;
    string msg=para->msg;
    para->state=false;
    para->returnMSG="-ERROR\r\n";
    Connect* conn = new Connect(socket);
    if(!conn->sendLine(socket,msg))
    {
        cout<<"send faild!";
    }

    string respone;
    if(conn->RecvLine(socket,respone))//收到请求。
    {
        cout<<"get response from participant:"<<respone<<endl;
        para->returnMSG=respone;
        para->msg=true;
    }
    close(socket);
    
    
}
MSG coordinator::RequestToParticipant(string msg)//将请求发送给参与者,协调者负责主动连接参与者。
{
    int len=pa_list.size();
    len=2;//debug用
    int connfd[len];//每与参与者建立一个请求，生成一个connfd.
    pthread_t req_th[len];//线程
    ThreadParas req_Para[len];//线程参数
    MSG returnMSG={.state=false,.message="-ERROR\r\n"};
    for(int i=0;i<len;i++)
    {
        connfd[i]=SocketApi::Connect_sock(pa_list[i].IP,pa_list[i].port);//把创建socket的过程也放里面了。返回创建的用于connect的socket
        ThreadParas req_args;
        req_Para[i].msg=msg;
        req_Para[i].socket=connfd[i];
        //pthread_create(&req_th[i], NULL, oneRequest, &req_Para[i]);
        if(pthread_create(&req_th[i], NULL, oneRequest, &req_Para[i])!=0)//create thread
        {
            perror("pthread_create failed");
        }
        //oneRequest(&req_Para[i]);                       
    }
    for(int i=0;i<len;i++)
        pthread_join(req_th[i],NULL);
    for(int i=0;i<len;i++)
    {
        if(!req_Para[i].state)
        {
            //该线程已断开，更改该线程的状态表值。
            TaskTable[i].TaskState=zombie;
        }
        else
        {
            cout<<"change participant TaskId from "<<TaskTable[i].TaskId<<" to "<<TaskId<<endl;
            TaskTable[i].TaskId=TaskId;
            returnMSG.state=true;
            returnMSG.message=req_Para[i].msg;
        }     
    }
    co_state=READY;
    cout<<"request finish!"<<endl;
    return returnMSG;
}//此函数需要统计多个线程的值是否正常，是否超时。

void coordinator::recvFromClient()
{
    int co_sock=SocketApi::Socket();
    SocketApi::Bind(co_sock,socketInfo.port,socketInfo.IP);
    SocketApi::Listen(co_sock);
    while(1)
	{
       std::string peer_ip;
       int peer_port;
       int sock = SocketApi::Accept(co_sock,peer_ip,peer_port);   
       std::cout << "part_sock :" << sock <<std::endl;
       if(sock>= 0)
       {
       	  std::cout << peer_ip << " : " << peer_port <<std::endl;
          Connect* part_conn = new Connect(co_sock);
          string line;
          part_conn->RecvLine(sock,line);
          cout<<"coordinator recv the request:"<<line<<endl;
          TaskId=TaskId+1;
          logwriter(to_string(TaskId)+" REQUEST "+line);
          //处理任务
          MSG msg=RequestToParticipant(line);
          if(msg.state)
          {
            logwriter(to_string(TaskId)+" FINISH "+line);
          }
          //发回给客户端。
          part_conn->sendLine(sock,msg.message);
       }
	}
}
bool coordinator::logwriter(string data)//向日志文件写入一行。
{
    // ofstream fileW;
    // fileW.open("./log/coordinator"+to_string(socketInfo.port)+".log",ios::app);//打开文件，用于在其尾部添加数据。如果文件不存在，则新建该文件。
    // //在文件夹下运行./kill.sh 将清空所有日志文件。
    // if(fileW.is_open())
    // {
    //     fileW<<data<<"\n";
    //     fileW.close();
    // }
    // else {cout<<"open error";}//有时候系统没有权限打开文件。
    log.push_back(data);
}
void coordinator::recvHeart()
{
    int co_sock=SocketApi::Socket();
    SocketApi::Bind(co_sock,socketInfo.port,socketInfo.IP);
    SocketApi::Listen(co_sock);
    while(1)
	{
       std::string peer_ip;
       int peer_port;
       int sock = SocketApi::Accept(co_sock,peer_ip,peer_port);   
       std::cout << "part_sock :" << sock <<std::endl;
       if(sock>= 0)
       {
       	  std::cout << peer_ip << " : " << peer_port <<std::endl;
          Connect* part_conn = new Connect(co_sock);
          string line;//接收到到字符串
          part_conn->RecvLine(sock,line);

          
       }
	}
}