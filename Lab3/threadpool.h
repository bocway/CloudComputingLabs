#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include<iostream>
#include<pthread.h>
#include<queue>
#include<map>
#include<string>

#include "write_log.hpp"
#define IF_DEBUG_PRINTF 0
class Participate;
typedef void (*Handler)(int,int,Log_record* ,std::multimap<std::string, int>);
typedef void (*Handler_part)(int,Log_record*,std::multimap<std::string, int>,Participate *);

class Task
{
	    
	protected:
	    int sock = 0;
        int thread = 0;
        Log_record *log_record;
        std::multimap<std::string, int> message;
        Handler handler;
	public:
        Task(int sock_,int thread_,Log_record *log_record_,std::multimap<std::string, int> message_,Handler handler_);
        Task(int sock_,Log_record *log_record_,std::multimap<std::string,int> message_);
        virtual void Run();
        ~Task();
};

class ParticipantTask : public Task
{
    private:
        Handler_part handler_part;
        Participate *participate;
    public:
        ParticipantTask(int sock_,Log_record *log_record_,std::multimap<std::string, int> message_,Handler_part handler_part_,Participate *participate_);
        void Run();
        ~ParticipantTask();
};

class ThreadPool
{
    private:
    	int num;
    	int idle_num;
    	std::queue<Task> task_queue;
    	pthread_mutex_t lock;
    	pthread_cond_t cond;
    public:
    	ThreadPool(int num_);
    	void PushTask(Task &t);
    	void Wakeup();
    	void Idle();
    	void InitThreadPool();
    	void LockQueue();
    	void UnlockQueue();
    	bool IsTaskQueueEmpty();
    	Task PopTask();
    	static void* ThreadRoutine(void* arg);
    	~ThreadPool();
};

class singleton
{
private:
	static ThreadPool* p;
	static pthread_mutex_t lock;
	int num_thread;

public:
	singleton(int num_thread_);
    ThreadPool* GetInstance();
    ~singleton();
};


#endif
