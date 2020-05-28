#include "coordinator.h"
coordinator::coordinator(Socket co_,vector<Socket> pa_list)
{
    socketInfo=co_;
    this->pa_list=pa_list;
    cout<<"create a coordinator "<<socketInfo.IP<<":"<<socketInfo.port<<endl;
    for(int i=0;i<pa_list.size();i++)
    {
        cout<<"painfo:"<<this->pa_list[i].IP<<":"<<this->pa_list[i].port<<endl;
    }
}
void coordinator::heart()
{
    
}