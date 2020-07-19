#ifndef _EPOOL_H_
#define _EPOOL_H_

#include <sys/types.h>
#include <sys/epoll.h>
#include "enum.h"

static int stans_fd;   //用于epoll传给add_worker

static struct epoll_event ev, events[EPOLL_SIZE];

int epoll_init();

#endif //_EPOOL_H_