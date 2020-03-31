#include <assert.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/time.h>
#include<queue>
#include "sudoku.h"

#define INPUT_JOB_NUM 30
int nextJobToBeDone=0;
int debug=0;
char puzzle[128];
int total_solved = 0;
int total = 0;

pthread_mutex_t jobqueueMutex=PTHREAD_MUTEX_INITIALIZER;

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}
/*
architecture：
  1.Dynamically assign job
  2.Threads do same tasks
  3.Long-lived worker thread pool
*/
typedef struct {
   int numOfsolve;
} ThreadParas;

typedef struct {
   char* fileName;
} ReadParas;

boardStruct recvAJob()
{

  boardStruct currentJob;

  sem_wait(&in_full); 
  pthread_mutex_lock(&in_mutex);
  if(q.size()!=0)
  {
    currentJob=q.front();
    q.pop();
    if(debug)
      printf("worker get num %d \n",currentJob.id);
  } 
  else currentJob.finish=true;//暂时结束

  pthread_mutex_unlock(&in_mutex);
  sem_post(&in_empty); 
  

  return currentJob;
}

void* myReadFunc(void* args){
  ReadParas* para = (ReadParas*) args;

  FILE* fp = fopen(para->fileName, "r");//argv【1】：open the file  
  while (fgets(puzzle, sizeof puzzle, fp) != NULL) {//get a input puzzle
    if (strlen(puzzle) >= N) {
      ++total;
      input(puzzle);
      //init_cache();
    }
  }
}

void* mysolve(void* args) {
  ThreadParas* para = (ThreadParas*) args;
  int sum=0;
  boardStruct currentJob;

  while(1)//相比preassign唯一的改动：算得越快，拿得越早
  {
    currentJob=recvAJob();//获得一个任务。 这个函数里面要加锁。
    if(currentJob.finish)//All job done!
    {
        sleep(1000);
        if(debug)
          printf("worker finish \n");
        //break;
    }
      
    
    // whichJobIHaveDone[numOfJobsIHaveDone]=currentJobID;
    // numOfJobsIHaveDone++;

    if(solve_sudoku_dancing_links(currentJob))
    {
      sum++;
      if(debug)
          printf("No: %d have been solved!\n", currentJob.id);
    }
    else {//haven‘t sloved this sudoku
        if(debug)
          printf("No: %d didn't solve!\n", currentJob.id);
      }
  }  
  para->numOfsolve=sum;
}
void printer(outStruct &o){
  if(debug)
    printf("No: %d :", o.id);
    for(int i=0;i<81;i++){
        printf("%d",o.board[i]);
    }
    printf("\n");
}
void* myOutFunc(void* args) {
  long int next_id=0;
  while(1)
  {
    sem_wait(&out_full); 
    pthread_mutex_lock(&out_mutex);

    map<long int,outStruct>::iterator iter=out.find(next_id);
    if(iter!=out.end())
    {
      printer(iter->second);
      next_id++;
      out.erase(iter);
    }  
    else//这轮没做功 释放锁和信号量
    {     
      pthread_mutex_unlock(&out_mutex);
      sem_post(&out_full); 
      continue;
    }

    pthread_mutex_unlock(&out_mutex);
    sem_post(&out_empty); 
  }
}
int main(int argc, char* argv[])
{
  init_neighbors();
  
  int numOfWorkerThread=3;//argv[3] get num of thread
  if(argv[2] != NULL)
    numOfWorkerThread=atoi(argv[2]);

  sem_init(&out_full, 0, 0); 
  sem_init(&out_empty, 0, 10000); 
  pthread_mutex_init(&out_mutex,NULL);

  sem_init(&in_full, 0, 0); 
  sem_init(&in_empty, 0, 10000); 
  pthread_mutex_init(&in_mutex,NULL); 

  int64_t start = now();
  //动态分配任务两种实现方式：
  //1.写一个分发线程分发给不同的worker
  //2.每次做完就从输入队列里拿
  pthread_t fileReader;
  pthread_t outputer;
  pthread_t th[numOfWorkerThread];

  ReadParas *rePara;
  rePara=(ReadParas*)malloc(sizeof(ReadParas)); //记得先分配内存空间。。
  rePara->fileName=(char*)argv[1];
  ThreadParas thPara[numOfWorkerThread];

  if(pthread_create(&fileReader, NULL, myReadFunc,rePara)!=0)
  {
    perror("fileReader pthread_create failed");
    exit(1);
  }

  
  for(int i=0;i<numOfWorkerThread;i++)
  {
    if(pthread_create(&th[i], NULL, mysolve, &thPara[i])!=0)
    {
      perror("pthread_create failed");
      exit(1);
    }
  }
  if(pthread_create(&outputer, NULL, myOutFunc,rePara)!=0)
  {
    perror("fileReader pthread_create failed");
    exit(1);
  }

  pthread_join(fileReader, NULL);
  for(int i=0;i<numOfWorkerThread;i++)
    pthread_join(th[i], NULL);
  pthread_join(outputer, NULL);

  for(int i=0;i<numOfWorkerThread;i++)
    total_solved=total_solved+thPara[i].numOfsolve;
  int64_t end = now();
  double sec = (end-start)/1000000.0;
  if(debug)
    printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);

  sem_destroy(&in_full); 
  sem_destroy(&in_empty); 
  pthread_mutex_destroy(&in_mutex);
  sem_destroy(&out_full); 
  sem_destroy(&out_empty); 
  pthread_mutex_destroy(&out_mutex);
  return 0;
}

