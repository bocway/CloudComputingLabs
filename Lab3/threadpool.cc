#include "threadpool.h"
#include "participant.h"
#include "protocolutil.h"

Task::Task(int sock_,int thread_,Log_record *log_record_,std::multimap<std::string,int> message_,Handler handler_)
    :sock(sock_),thread(thread_),log_record(log_record_),message(message_),handler(handler_)
{}
Task::Task(int sock_,Log_record *log_record_,std::multimap<std::string,int> message_)
    :sock(sock_),log_record(log_record_),message(message_)
{}
void Task::Run()
{
	handler(sock,thread,log_record,message);
}
Task::~Task()
{}

ParticipantTask::ParticipantTask(int sock_, Log_record *log_record_,std::multimap<std::string,int> message_,Handler_part handler_part_,Participate *participate_)
    :Task(sock_, log_record_, message_), handler_part(handler_part_), participate(participate_)
{}
void ParticipantTask::Run()
{
    handler_part(sock,log_record,message,participate);
}
ParticipantTask::~ParticipantTask()
{}

ThreadPool::ThreadPool(int num_):num(num_),idle_num(0)
{
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cond,NULL);
}
void ThreadPool::PushTask(Task &t)
{ 
	pthread_mutex_lock(&lock);
	task_queue.push(t);
    pthread_mutex_unlock(&lock);
    Wakeup();
}
void ThreadPool::Wakeup()
{
	pthread_cond_signal(&cond);
}
void ThreadPool::Idle()
{
	idle_num ++;
	pthread_cond_wait(&cond,&lock);
	idle_num --;
}
void ThreadPool::InitThreadPool()
{
	pthread_t tid;
	for(auto i = 0;i< num;i++)
	{
		pthread_create(&tid,NULL,ThreadRoutine,(void*)this);
	}
}
void ThreadPool::LockQueue()
{
    pthread_mutex_lock(&lock);
}
void ThreadPool::UnlockQueue()
{
	pthread_mutex_unlock(&lock);
}
bool ThreadPool::IsTaskQueueEmpty()
{
	return task_queue.empty();
}

Task ThreadPool::PopTask()
{
    Task t = task_queue.front();
    task_queue.pop();
    return t;
}

void* ThreadPool::ThreadRoutine(void* arg)
{
	pthread_detach(pthread_self());
    ThreadPool* tp = (ThreadPool*)arg;

	for(;;)
    {
    	tp->LockQueue();
        while(tp->IsTaskQueueEmpty())
        {
        	tp->Idle();
        }
        Task t = tp->PopTask();
    	tp->UnlockQueue();   
    	if(IF_DEBUG_PRINTF)
    	   std::cout<< "task is handler by: " << pthread_self() << std::endl;
    	t.Run();
    }
}
ThreadPool::~ThreadPool()
{
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
}

singleton::singleton(int num_thread_):num_thread(num_thread_){}
ThreadPool* singleton::GetInstance()
{
	if(NULL == p)
	{
		pthread_mutex_lock(&lock);
		if(NULL == p)
		{
			p = new ThreadPool(num_thread);
			p->InitThreadPool();

		}
		pthread_mutex_unlock(&lock);
	}
	return p;
}
singleton::~singleton()
{}

ThreadPool *singleton::p = NULL;
pthread_mutex_t singleton::lock = PTHREAD_MUTEX_INITIALIZER;