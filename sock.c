#include "sock.h"
#include "enum.h"

void setnonblocking(int sock)
{
	int opts = fcntl(sock, F_GETFL, 0);
	if (opts<0)
	{
		perror("fcntl(sock, F_GETFL)");
		exit (1);
	}
	opts = opts | O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0)
	{
		perror("fcntl(sock, F_GETFL, opts)");
		exit(1);
	}
}

int sock_init()
{
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listen_socket)
	{
		perror("创建套接字");
		exit(-1);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));            
	addr.sin_family      = AF_INET;            // Internet地址族 
	addr.sin_port        = htons(PORT);        // 端口号    使用大端模式
	addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 绑定本地 任意 ip 地址, 本地的所有ip地址都可用 
	int ret = bind(listen_socket, (const struct sockaddr *)&addr, sizeof(addr));
	if (-1 == ret)
	{
		perror("bind");
		exit(-1);
	}
	
	listen(listen_socket, 5);
	
	
	return listen_socket;
}

int myAccept(int listen_socket)
{
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	int client_socket = accept(listen_socket, (struct sockaddr *)&client_addr, (socklen_t *)&len);
	if (-1 == client_socket)
	{
		perror("accept");
		return -1;
	}

	printf ("接收一个客户端连接,  ip = %s, port = %d\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

	return client_socket;	
}