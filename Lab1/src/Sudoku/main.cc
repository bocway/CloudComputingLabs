#include <assert.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "sudoku.h"

#define INPUT_JOB_NUM 30
int nextJobToBeDone=0;
int debug=0;
char puzzle[128];
int total_solved = 0;
int total = 0;

pthread_mutex_t jobQueueMutex=PTHREAD_MUTEX_INITIALIZER;

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

int recvAJob()
{
  int currentJobID=0;
  pthread_mutex_lock(&jobQueueMutex);//加锁
  if(nextJobToBeDone>=INPUT_JOB_NUM) //判断任务是否全部做完，做完则解锁推出
  {
    pthread_mutex_unlock(&jobQueueMutex);
    return -1;
  }
  currentJobID=nextJobToBeDone;//当前任务id设置为下一个
  nextJobToBeDone++;
  pthread_mutex_unlock(&jobQueueMutex);
  return currentJobID;
}

void* myReadFunc(void* args){
  ReadParas* para = (ReadParas*) args;
  if(debug)
    printf("para= %s",para->fileName);
  
  FILE* fp = fopen(para->fileName, "r");//argv【1】：open the file  
  while (fgets(puzzle, sizeof puzzle, fp) != NULL) {//get a input puzzle
    if (strlen(puzzle) >= N) {
      ++total;
      input(total,puzzle);
      //init_cache();
    }
  }
}

void* mysolve(void* args) {
  ThreadParas* para = (ThreadParas*) args;
  int sum=0;
  int currentJobID=0;
  int *whichJobIHaveDone=(int*)malloc(INPUT_JOB_NUM*sizeof(int));//Remember which job I have done
  long int numOfJobsIHaveDone=0;//Remember how many jobs I have done
  while(1)//相比preassign唯一的改动：算得越快，拿得越早
  {
    currentJobID=recvAJob();//获得一个任务。 这个函数里面要加锁。
    if(currentJobID==-1)//All job done!
      break;

    whichJobIHaveDone[numOfJobsIHaveDone]=currentJobID;
    numOfJobsIHaveDone++;

    if(solve_sudoku_basic(currentJobID,0))
    {
      sum++;
      if(debug)
      {
        printf("No: %d have been solved\n", currentJobID);
      }     
    }
    else {//haven‘t sloved this sudoku
        if(debug)
          printf("No: %d didn't solve!\n", currentJobID);
      }
  }  
  para->numOfsolve=sum;
}

int main(int argc, char* argv[])
{
  init_neighbors();

  bool (*solve)(int,int) = solve_sudoku_basic;

  // if (argv[2] != NULL)//argv[2] get sudoku solution
  //   if (argv[2][0] == 'a')
  //     solve = solve_sudoku_min_arity;
  //   else if (argv[2][0] == 'c')
  //     solve = solve_sudoku_min_arity_cache;
  //   else if (argv[2][0] == 'd')
  //     solve = solve_sudoku_dancing_links;
  
  int numOfWorkerThread=3;//argv[3] get num of thread
  if(argv[2] != NULL)
    numOfWorkerThread=atoi(argv[2]);

  int64_t start = now();
  //动态分配任务两种实现方式：
  //1.写一个分发线程分发给不同的worker
  //2.每次做完就从输入队列里拿
  pthread_t fileReader;
  pthread_t th[numOfWorkerThread];

  ReadParas *rePara;
  rePara=(ReadParas*)malloc(sizeof(ReadParas)); //记得先分配内存空间。。
  rePara->fileName=(char*)argv[1];
  ThreadParas thPara[numOfWorkerThread];
  printf("para=%s",rePara->fileName);

  if(pthread_create(&fileReader, NULL, myReadFunc,rePara)!=0)
  {
    perror("fileReader pthread_create failed");
    exit(1);
  }

  pthread_join(fileReader, NULL);
  for(int i=0;i<numOfWorkerThread;i++)
  {
    if(pthread_create(&th[i], NULL, mysolve, &thPara[i])!=0)
    {
      perror("pthread_create failed");
      exit(1);
    }
  }
  
  for(int i=0;i<numOfWorkerThread;i++)
    pthread_join(th[i], NULL);
  
  for(int i=0;i<numOfWorkerThread;i++)
    total_solved=total_solved+thPara[i].numOfsolve;
  int64_t end = now();
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);

  return 0;
}

