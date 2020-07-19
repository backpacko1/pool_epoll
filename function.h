#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "enum.h"


    static sqlite3* db = NULL;
    static char** result = NULL;
	static char* errmssg = NULL;

struct USR
{
	int sockfd;
	int group_online;
	char name[15];
};

struct information
{
	int sockfd;
	int choose;
	int sem;
	int group;
	int private;
	int client_channel;
	char nickname[15];
	char password[30];
	char pri_name[15];
	char info[50];
	struct USR usr[MAX_SIZE];
};

extern struct information usr;

void login(struct information* news);
void registered(struct information* news);
void*handle_client(void*attr);
int init ();
int myAccept(int listen_socket);
void group_chat(struct information* news);
void private_chat(struct information* news);
void view_online(struct information* news);
//void view_recording(struct information* news);
void registered_login(struct information* news);
void exit_login(struct information* news);

#endif //_FUNCTION_H_