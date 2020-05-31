#include "protocolutil.h"
const char* ErrLevel[] = {
	"Normal",
	"Warning",
	"Error"
};

int SocketApi::Socket()
{
	std::cout << "socket is ready..." << std::endl;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		//LOG("socket error!",ERROR);
		std::cout<<"socket error!"<<std::endl;
		exit(2);
	}
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	return sock;
}
void SocketApi::Bind(int sock,int port,std::string SERV_ADDR)
{
	std::cout << "bind is ready..." << std::endl;
	struct sockaddr_in local;
	bzero(&local,sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(SERV_ADDR.c_str());

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		std::cout<<"bind error!"<<std::endl;//LOG("bind error!",ERROR);
		exit(3);
	}		
}
void SocketApi::Listen(int sock)
{
	std::cout << "listen is ready..." << std::endl;
	if(listen(sock,BACKLOG) < 0)
	{
		//LOG("listen error",ERROR);
		std::cout<<"listen error"<<std::endl;
		exit(4);
	}
}

int SocketApi::Accept(int listen_sock,std::string& ip,int &port)
{
	std::cout << "accept is ready..." << std::endl;
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	int sock = accept(listen_sock,(struct sockaddr*)&peer,&len);
	if(sock < 0)
	{
		std::cout << "accept error" << std::endl;
		return -1;
	}
	port = ntohs(peer.sin_port);
	ip = inet_ntoa(peer.sin_addr);
	return sock;
}
int SocketApi::Connect_sock(std::string SERV_ADDR,int SERV_PORT)
{
	
    std::cout << "connect is ready...." << std::endl;

    struct sockaddr_in serverAdd;

    bzero(&serverAdd, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = inet_addr(SERV_ADDR.c_str());
    serverAdd.sin_port = htons(SERV_PORT);
    
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
 
    int connResult = connect(connfd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (connResult < 0) 
    {
        std::cout << "connect error" << std::endl;//LOG("connect error!",WARNING);
    }
    return connfd;
}



/*class Connect//recv&send data
{
*/	
void Connect::setState(std::string state_)
{
	state = state_;
}
std::string Connect::getState()
{
	return state;
}
bool Connect::RecvLine(int sock_,std::string& line)
{
	char buff[BUFF_NUM];
	ssize_t s = recv(sock_,buff,BUFF_NUM,0);
    buff[strlen(buff)] = '\0';
    line = buff;
    std::cout<<"recv line:" << line <<" s:"<<s<<std::endl;
    return (s > 0) ? true:false;
}
bool Connect::sendLine(int connfd,std::string line)
{
    ssize_t s = send(connfd,line.c_str(),line.size(),0);
    std::cout << "send line:" <<line << " s:" <<s <<std::endl;
    return (s > 0) ? true:false;
}