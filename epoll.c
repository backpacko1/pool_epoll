#include "epoll.h"
#include "sock.h"
#include "enum.h"
#include "pool.h"
#include "function.h"
#include <errno.h>


//用于保存向客户端发送一次消息所需的相关数据
//struct user_data
//{
//    int fd;
//    unsigned int n_size;
//    char line[MAXLINE];
//};
struct information usr;


int epoll_init()
{
	int epfd;
	int nfds;
	
	
	epfd = epoll_create(256);
	
	int listen_fd = sock_init();
	//setnonblocking(listen_fd);
	
	ev.data.fd = listen_fd;
	ev.events = EPOLLIN | EPOLLET;
	
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd,&ev);
	if (errno)
	{
		perror("epoll_ctl");
	}
	while(1)
	{
		int i = 0;
		int connfd;
		int fd;
		nfds = epoll_wait(epfd, events, EPOLL_SIZE, -1);
		printf("nfds = %d\n",nfds);
		if (errno)
		{
			perror("epoll_wait");
		}
		
		for (i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == listen_fd)
			{				
				connfd = myAccept(listen_fd);
				//setnonblocking(connfd);
				
				ev.data.fd = connfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
	            if (errno)
	            {
	            	perror("epoll_ctl");
	            }	
				
			}
			else if (events[i].events & EPOLLIN)
			{	
		printf("EPOLLIN  fd = %d   i = %d\n", events[i].data.fd, i);
				stans_fd = events[i].data.fd;
				add_worker(handle_client, (void*)(&stans_fd));
				//ev.data.fd = stans_fd;
				//ev.events = EPOLLOUT | EPOLLET;
				//epoll_ctl(epfd, EPOLL_CTL_MOD, stans_fd, &ev);
			}
			else if (events[i].events & EPOLLOUT)
			{
				printf("EPOLLOUT  fd = %d   i = %d\n", events[i].data.fd, i);
				//stans_fd = events[i].data.fd;
				//ev.data.fd = stans_fd;
				//ev.events = EPOLLIN | EPOLLET;
				//epoll_ctl(epfd, EPOLL_CTL_MOD, stans_fd, &ev);
				//stans_fd = events[i].data.fd;
				//add_worker(handle_client, (void*)&stans_fd);
			}
		}
	}
	
}




