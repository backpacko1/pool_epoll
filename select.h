#ifndef _SELECT_H_
#define _SELECT_H_

#include <sys/select.h>
#include <stdio.h>


int select_init();
void handl_client(int sfd, fd_set *allset);

#endif //_SELECT_H_