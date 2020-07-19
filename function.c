#include "function.h"




void*handle_client(void*attr)        //处理客户端线程函数
{
	struct information news;
	int client_socket= *(int*)attr;
	char tmp[3072];
	int ret = 0;
	int m = 0;
	
	printf("sizeof(struct information) = %d\n",sizeof(struct information));
	printf("already come in handle_client   fd = %d\n", client_socket);
	
	//while(1)
	//{
		memset(&news,0,sizeof(struct information));
		memset(tmp,0,sizeof(tmp));
		
		ret = read(client_socket,tmp,sizeof(struct information));
		printf("ret = %d\n",ret);
		
		memcpy(&news, tmp, sizeof(struct information));
		
		news.sockfd = client_socket;
		printf("news.choose = %d\n", news.choose);
		switch(news.choose)
		{
			case REGISTERED:
				registered(&news);   //注册 nickname和password写入数据库
				break;
			case LOGIN:
				login(&news);        //登录成功 状态写入usr
				break;
			case VIEW_ONLINE:
				view_online(&news);  //发送状态结构体usr 
				break;				
			case GROUP_CHAT:
				group_chat(&news);   //根据group_online选择性发送
				break;	
			case PRIVATE_CHAT:
				private_chat(&news);  //根据数据库和usr
				break;
			//case 4:view_recording(&news);
				//break;
			case 5:
				break;
			case EXIT_LOGIN:
				exit_login(&news);
				m = 1;
				break;	
			default :
				break;
		}
		
		//if(m)
		//	break;

	//}
	return NULL;

}

void registered(struct information* news)      //用户注册子函数
{
	int ret;
	char nickname[30];
	char password[30];
	
	int i = 0;
	int pnRow;
	int pnColumn;
	char sql[100];
	char* sqll;
	
	
	
		ret = sqlite3_open("account_date.db",&db);
		if(ret != SQLITE_OK)
		{
			printf("数据库打开失败\n");
		}
		
		sqll = "create table if not exists list1(nickname TEXT,password TEXT,primary key(nickname));";
		
		ret = sqlite3_exec(db,sqll,NULL,NULL,&errmssg);
		if(ret != SQLITE_OK)
		{
			printf("table已存在，不需要再次创建:%s\n",errmssg);
		sqlite3_free(errmssg);
		}
			
	
		sprintf(sql,"insert into list1 values('%s','%s')",news->nickname,news->password);
		ret = sqlite3_exec(db,sql,NULL,NULL,&errmssg);
		if(ret != SQLITE_OK)
		{
			sqlite3_close(db);
			news->sem = REG_FAILURE;       //注册失败
			sqlite3_close(db);
			printf("%s\n",errmssg);
			send(news->sockfd,(char*)news,sizeof(struct information),0);
			return;
		}
		news->sem = REG_SUCCESS;         //注册成功
		send(news->sockfd,(char*)news,sizeof(struct information),0);
		sqlite3_close(db);
		
	printf("%d\n",news->sem);	
	
	
}

void login(struct information* news)           //用户登录子函数
{
	int ret;
	char nickname[30];
	char password[30];
	
	int i,j;
	int pnRow;
	int pnColumn;
	char sql[100];

	
	
	
	ret = sqlite3_open("account_date.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("数据库打开失败\n");
	}
	
	sprintf(sql,"select password from list1 where nickname='%s'",news->nickname);
	errmssg = NULL;
	ret = sqlite3_get_table(db,sql,&result,&pnRow,&pnColumn,&errmssg);

	if(ret != SQLITE_OK)        //登录失败  nickname不一样
	{
	
		printf("账户或者密码错误 %s\n",errmssg);
		sqlite3_free(errmssg);
		sqlite3_close(db);
		news->sem = ACC_PASS_ERR;
		send(news->sockfd,(char*)news,sizeof(struct information),0);
		return;			
	}
	        //登录成功
	
	for(i=0; i<pnColumn;i++)
	{
		
		if (strcmp(news->password,result[pnColumn + i]) == 0)
	       {
			news->sem = LOGIN_SUCCESS;
			send(news->sockfd,(char*)news,sizeof(struct information),0);
			sqlite3_close(db);
			for(j=0; j<MAX_SIZE;j++)
			{
				if(usr.usr[j].sockfd == 0)
				{
					memcpy(usr.usr[j].name,news->nickname,sizeof(usr.usr[j].name));
					usr.usr[j].sockfd = news->sockfd;
				//	printf("%d\n",usr.usr[j].sockfd);
					break;
					
				}
			}
			return;
		}
		else
		{
			printf("账户或者密码错误 %s\n",errmssg);  //密码不正确
			sqlite3_free(errmssg);
			sqlite3_close(db);
			news->sem = ACC_PASS_ERR;
			send(news->sockfd,(char*)news,sizeof(struct information),0);
			break;
		}
		
	}
	
}

void view_online(struct information* news)     //查看在线人数
{
	int i = 0;
	
	usr.client_channel = VIEW_ONLINE;
	send(news->sockfd,(char*)&usr,sizeof(struct information),0);

}

void group_chat(struct information* news)      //要遍历所有在线客户端的group_online
{
	int j = 0;
	int ret;

	news->client_channel = GROUP_CHAT;
		
	if((news->group) == 1)  //客户进入聊天室group_online置1
	{
		while(j<MAX_SIZE)
		{
			if(memcmp(usr.usr[j].name,news->nickname,sizeof(usr.usr[j].name)) == 0)
			{
				usr.usr[j].group_online = 1;
				break;
			}
			j++;
		}
	
	}	
	
	j = 0;
	if(news->group == 0)  //客户退出聊天室group_online置0
	{
		while(j<MAX_SIZE)
		{
			if(memcmp(usr.usr[j].name,news->nickname,sizeof(usr.usr[j].name)) == 0)
			{
				usr.usr[j].group_online = 0;
				break;
			}
			j++;
		}
	}
	
	printf("%s",news->info);
	j = 0;
	while(j < MAX_SIZE)
	{         					//发给所有group_online
		if( (usr.usr[j].sockfd != 0) && (usr.usr[j].group_online == 1) )
			ret = send(usr.usr[j].sockfd,(char*)news,sizeof(struct information),0);
		j++;
	}
		
	
}

void private_chat(struct information* news)    //
{
	int ret;
	int r,p;
	char sql[50];
	int i,j;

	news->client_channel = PRIVATE_CHAT;

    ret = sqlite3_open("account_date.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("数据库打开失败\n");
	}
	sprintf(sql,"select * from list1 where nickname='%s'",news->pri_name);
	ret = sqlite3_get_table(db,sql,&result,&r,&p,&errmssg);
	if(ret != SQLITE_OK)   //告诉客户端输入了错误nickname    
	{
		printf("用户不存在 %s\n",errmssg);
		news->sem = NO_USER;
		send(news->sockfd,(char*)news,sizeof(struct information),0);
		return;
	}
	sqlite3_close(db);

    if(news->sem == CLIENT_EXIT)//客户端退出 告诉目的用户
	{
		for(j=0; j<MAX_SIZE; j++)
		{
			if(memcmp(news->pri_name,usr.usr[j].name,sizeof(news->pri_name)) ==0)
			{
				news->sem = CLIENT_EXIT;//通知目的用户  对方客户端已离开私聊 （用来以后建立P to P聊天）
				send(usr.usr[j].sockfd,(char*)news,sizeof(struct information),0);
				break;
			}
		}
		return;	
	}
	
	for(i=0; i<MAX_SIZE; i++)//数据库有目的用户nickname 开始遍历usr找usr.usr[].name
	{
		if(memcmp(news->pri_name,usr.usr[i].name,sizeof(news->pri_name)) ==0)
		{
			news->sem = 333;//找到后向目的用户转发客户端消息
			send(usr.usr[i].sockfd,(char*)news,sizeof(struct information),0);
			break;			
		}				
	
	}
	if(i == MAX_SIZE)   //usr中没有目的用户 说明不在线
	{
		printf("news->sockfd = %d\n", news->sockfd);
		news->sem = 222;  //通知客户端 目的用户离线状态
		send(news->sockfd,(char*)news,sizeof(struct information),0);	
	}

}

void exit_login(struct information* news)      //客户端退出要重置其状态
{
	int i = 0;
	
	while(i<MAX_SIZE)
	{
		if(news->sockfd == usr.usr[i].sockfd)
		{
			memset(usr.usr[i].name,0,sizeof(usr.usr[i].name));
			usr.usr[i].sockfd = 0;
			break;
		}
	}
	close(news->sockfd);
	
}
