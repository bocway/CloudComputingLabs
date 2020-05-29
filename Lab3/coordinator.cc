#include "coordinator.h"
coordinator::coordinator(Socket co_,vector<Socket> pa_list)
{
    socketInfo=co_;
    this->pa_list=pa_list;
    cout<<"create a coordinator "<<socketInfo.IP<<":"<<socketInfo.port<<endl;
    for(int i=0;i<pa_list.size();i++)
    {
        cout<<"painfo:"<<this->pa_list[i].IP<<":"<<this->pa_list[i].port<<endl;
        //状态表初始化：状态均为init 事务id为0
        TaskTableItem tmp={.TaskId=0,.TimeStamp=0,.TaskState=init};
        TaskTable.push_back(tmp);
    }

}
MSG coordinator::RequestToParticipant(string msg)//将请求发送给参与者,协调者负责主动连接参与者。
{
    int len=pa_list.size();
    len=2;//debug用
    int connfd[MAX_PARTICIPANT];//每与参与者建立一个请求，生成一个connfd.
    //改成多线程连接。一个socket后续还有多个操作- -
    for(int i=0;i<len;i++)
    {
        connfd[i]=SocketApi::Connect_sock(pa_list[i].IP,pa_list[i].port);//把创建socket的过程也放里面了。返回创建的用于connect的socket
        Connect* conn = new Connect(connfd[i]);
        if(!conn->sendLine(connfd[i],msg))
        {
            cout<<"send faild!";
        }               
    }
    co_state=READY;
    cout<<"request finish!"<<endl;
}

void coordinator::recvFromClient()
{
    int co_sock=SocketApi::Socket();
    SocketApi::Bind(co_sock,socketInfo.port);
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
          //RequestToParticipant(line);
       }
	}
}
bool coordinator::logwriter(string data)//向日志文件写入一行。
{
    ofstream fileW;
    fileW.open("./log/coordinator"+to_string(socketInfo.port)+".log",ios::app);//打开文件，用于在其尾部添加数据。如果文件不存在，则新建该文件。
    //在文件夹下运行./kill.sh 将清空所有日志文件。
    if(fileW.is_open())
    {
        fileW<<data<<"\n";
        fileW.close();
    }
    else {cout<<"open error";}//有时候系统没有权限打开文件。
}
void coordinator::recvHeart()
{
    
}