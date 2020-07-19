#ifndef _SOCK_H_
#define _SOCK_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>



int sock_init();
int myAccept();
void setnonblocking(int sock);

#endif //_SOCK_H_