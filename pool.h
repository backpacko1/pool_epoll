#ifndef _POOL_H_
#define _POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



typedef struct worker
{
	void *(*process)(void *);     // 回调函数指针
	void *arg;                    // 回调函数参数
	int finish;
	struct worker *next;
} CThread_worker;

/*线程池结构体*/
typedef struct
{
	pthread_mutex_t queue_lock;
	pthread_cond_t queue_ready;
	
	CThread_worker *queue_head;
	
	int shutdown;
	pthread_t *threadid;
	
	int max_thread_num;
	int cur_queue_size;
} CThread_pool;

static CThread_pool *pool = NULL;

void pool_init(int max_thread_num);
void *thread_routine(void *arg);
int add_worker(void*(*process)(void *arg), void *arg);
int pool_destroy();





#endif //_POOL_H_