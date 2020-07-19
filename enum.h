#ifndef _ENUM_H_
#define _ENUM_H_

/* 宏常量*/

#define PORT              9000          //通信端口
#define BUF_SIZE          1024          //selct buf_size
#define MAX_THREAD_NUM    10      	    //线程池线程数量
#define MAX_SIZE          50            //在线列表
#define EPOLL_SIZE        50            //epoll每次能处理的事件数

/*  handle函数相关  */

#define VIEW_ONLINE       1				//查看在线人数
#define GROUP_CHAT        2				//群聊
#define PRIVATE_CHAT      3				//私聊
#define VIEW_RECORDING    4				//待定
#define BB                5				//待定
#define EXIT_LOGIN        6				//退出登录
#define CC                7             //待定
#define REGISTERED        8				//注册
#define LOGIN             9				//登录

/* 登录注册 */
#define INIT               0              // 初始值
#define REG_SUCCESS        44             //注册成功
#define REG_FAILURE        33             //注册失败
#define ACC_PASS_ERR       66             //账户或者密码错误
#define LOGIN_SUCCESS      77             //登录成功

/*群聊私聊*/
#define NO_USER            444			   //用户不存在
#define CLIENT_EXIT        111             //客户端退出 告诉目的用户
















#endif //_ENUM_H_