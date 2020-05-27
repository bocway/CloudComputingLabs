#include <assert.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <queue>
#include "KVserver.h"
#include "coordinator.h"
#include "participant.h"


int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}



int main(int argc, char* argv[])
{
  //获取参数并解析，构建相应的对象。
  participant a("10.0.5.1","8001");
  coordinator b("127.0.0.1","8002");

}

