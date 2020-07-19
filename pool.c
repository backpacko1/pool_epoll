#include "pool.h"
#include "function.h"
#include "sock.h"




void pool_init(int max_thread_num)
{
	int i = 0;
	pool = (CThread_pool*)malloc(sizeof(CThread_pool));
	if(!pool)
		printf("pool_init malloc error\n");
	
	pthread_mutex_init(&(pool->queue_lock), NULL);
	pthread_cond_init(&(pool->queue_ready), NULL);
	
	pool->queue_head = NULL;
	
	pool->shutdown = 0;
	/*为线程的pid开辟空间*/
	pool->threadid = (pthread_t*)malloc(max_thread_num*sizeof(pthread_t));
	if(!pool)
		printf("pool_init malloc error\n");
	
	pool->max_thread_num = max_thread_num;
	pool->cur_queue_size = 0;
	
	for (i = 0; i<max_thread_num; i++)
	{
		pthread_create(&(pool->threadid[i]), NULL, thread_routine, NULL);
	}
}

void *thread_routine(void *arg)
{
	printf("starting thread 0x%x \n",pthread_self());
	CThread_worker *worker;
	while(1)
	{
		pthread_mutex_lock(&(pool->queue_lock));
		/*任务队列里没有任务就睡眠，或线程池没有要销毁*/
		while(pool->cur_queue_size == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
		}
		/*如果是被线程池要销毁的信号唤醒*/
		if (pool->shutdown)
		{
			/*线程退出前一定要把锁解了*/
			pthread_mutex_unlock(&(pool->queue_lock));
			pthread_exit(NULL);
		}
		/*任务队列里有任务*/
		printf("thread 0x%x is starting to work\n", pthread_self());
		//asset(pool->cur_queue_size != 0);  //调试函数
		//asset(pool->queue_head != NULL);
		
		pool->cur_queue_size--;
		worker = pool->queue_head;
		pool->queue_head = pool->queue_head->next;
		pthread_mutex_unlock(&(pool->queue_lock));
		
		/*调用回调函数执行任务*/
		//int fd = *(int *)(worker->arg);  //调试
		//printf("recive fd = %d\n", fd);
		//*(worker->process)(worker->arg);
		worker->process(worker->arg);
		free(worker);
	}
	pthread_exit(NULL);
}

int add_worker(void*(*process)(void*), void *arg)
{
	//tips  调试
	//printf("one new worker ready to add  fd = %d\n",*(int*)arg);
	
	CThread_worker *newworker = (CThread_worker*)malloc(sizeof(CThread_worker));
	newworker->process = process;
	newworker->arg = arg;
	newworker->next = NULL;
	
	/*将新任务加入任务队列*/
	pthread_mutex_lock(&(pool->queue_lock));
	CThread_worker *member = pool->queue_head;
	
	if (member)     //每次加新任务都要遍历到链表尾
	{
		while(member->next)
			member = member->next;
		member->next = newworker;
	}
	else
		pool->queue_head = newworker;
	//asset(pool->queue_head != NULL);
	pool->cur_queue_size++;
	pthread_mutex_unlock(&(pool->queue_lock));
	pthread_cond_signal(&(pool->queue_ready));
	return 0;

}	

int pool_destroy()
{
	if (pool->shutdown)
		return -1;
	pool->shutdown = 1;   // 这里不加锁会不会有问题
	pthread_cond_broadcast(&(pool->queue_ready));    //唤醒所有进程
	int i;
	for (i = 0; i<pool->max_thread_num; i++)
		pthread_join(pool->threadid[i], NULL);
	free(pool->threadid);
	
	CThread_worker* worker = NULL;
	while(pool->queue_head != NULL)
	{
		worker = pool->queue_head;
		pool->queue_head = pool->queue_head->next;
		free(worker);
	}
	pthread_mutex_destroy(&(pool->queue_lock));
	pthread_cond_destroy(&(pool->queue_ready));
	free(pool);
	pool = NULL;
	return 0;
}








