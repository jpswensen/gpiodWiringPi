#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "gpiodWiring.h"

typedef void* ThreadFun(void* param);

#define SCHEDULER_POLICY    SCHED_FIFO

static long tickCnt = 0;

#define PIN 2

static void* tickThread(void *param)
{

  while (1)
  {
    tickCnt++;
    digitalWrite(PIN, tickCnt%2);

    struct timespec res;
    res.tv_sec = 0;
    res.tv_nsec = 750000;
    clock_nanosleep(CLOCK_REALTIME, 0, &res, NULL); 
  }
}

static void* workerThread(void *param)
{
  while (1)
  {
    struct timespec res;
    res.tv_sec = 0;
    res.tv_nsec = 15000000;
    clock_nanosleep(CLOCK_REALTIME, 0, &res, NULL);
  }
}

static int createThread(char const* funcName, ThreadFun* func, int prio)
{
  pthread_t tid = 0;
  pthread_attr_t threadAttr;
  struct sched_param schedParam;

  printf("thread create func=%s prio=%d\n", funcName, prio);

  pthread_attr_init(&threadAttr);
  pthread_attr_setschedpolicy(&threadAttr, SCHEDULER_POLICY);
  pthread_attr_setinheritsched(&threadAttr, PTHREAD_EXPLICIT_SCHED);
  schedParam.sched_priority = prio;
  pthread_attr_setschedparam(&threadAttr, &schedParam);

  if (pthread_create(&tid, &threadAttr, func, NULL) != 0)
  {
    printf("Error creating thread: %d\n", -1);
    return -1;
  }

  printf("thread created func=%s prio=%d\n", funcName, prio);
  return 0;
}
#define CREATE_THREAD(func,prio)  createThread(#func,func,prio)


int main(int argc, char*argv[])
{
  int minPrio = sched_get_priority_min(SCHEDULER_POLICY);
  int maxPrio = sched_get_priority_max(SCHEDULER_POLICY);
  int prioRange = maxPrio - minPrio;

  pinMode(PIN,OUTPUT);
  
  CREATE_THREAD(tickThread, maxPrio);
  CREATE_THREAD(workerThread, minPrio + prioRange / 4);
  sleep(10);
  printf("%ld ticks\n", tickCnt);
}
