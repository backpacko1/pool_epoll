#include "pool.h"
#include "sock.h"
#include "enum.h"
#include "epoll.h"



int main(int argc, char**argv)
{
	//初始化线程池
	pool_init(MAX_THREAD_NUM);
	
	//初始化select
	epoll_init();
	//select_init();
}