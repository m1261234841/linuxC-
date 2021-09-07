﻿#include <cstdio>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>

static int num = 0;
void* tfunc(void* arg) {
	std::cout << "create a thread" << std::endl;
	int* a = (int*)arg;
	for (int i = 0; i < *a; i++)
	{
		std::cout << "in child thread:" << i << std::endl;
	}
}
int Bind(int port, const char* IP)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_port = htons(port);
    if (IP == nullptr)
        addr.sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, IP, &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret >= 0);
    return lfd;
}
void free_process(int sig) {
	pid_t pid;
	while (1) {
		pid = waitpid(-1, nullptr, WNOHANG);
		if (pid <= 0) // <0 没有要等待的进程， =0 子进程没有退出
			break;
		else
		{
			printf("child process pid %d\n", pid);
		}
	}
}
void handler1(int sig)
{
	pid_t pid;
	while (pid = waitpid(-1, nullptr, WNOHANG) > 0)
	{
		std::cout << "an child process terminated\n";
	}
}
int main()
{
    //---------------------------------------------------------------------------------------------------
    //? 网络字节序转换  htonl()  ntohl()

    /*char* buf = "192.168.1.1";
    int num = *(int*)buf;
    int turned_num = htonl(num);
    buf = (char*)&turned_num;*/
    //---------------------------------------------------------------------------------------------------


	//---------------------------------------------------------------------------------------------------
	/**
	* typedef void(*sighandler_t)(int)
	* 信号函数signal(int sig, sighandler_t handler);
	*/
	// 信号注册
	//signal(SIGINT, handler1); // ctrl+c
	//signal(SIGQUIT, handler1); // ctrl+'\'
	//while(1)
	//{
	//	// std::cout << "in while" << std::endl;
	//	sleep(1);
	//}
	//---------------------------------------------------------------------------------------------------


	//---------------------------------------------------------------------------------------------------
	// 信号集： 阻塞信号集 未决信号集 通过“位图”实现 共32位
	// 自定义信号集合函数:
	//? int sigfillset(sigset_t* set) 将所有信号加入集合
	//? int sigemptyset(sigset_t* set) 将集合置空
	//? int sigaddset(sigset_t* set, int signo) 将signo信号加入集合
	//? int sigdelset(sigset_t* set, int signo) 将signo信号删除
	//? int sigismember(sigset_t* set, int signo) 判断信号是否存在 
	//! 示例程序
	//sigset_t set;
	//int i = 0;
	//sigemptyset(&set);
	//for (i = 1; i < 32; i++)
	//{
	//	if (sigismember(&set, i)) {
	//		std::cout << '1';
	//	}
	//	else
	//	{
	//		std::cout << '0';
	//	}
	//}
	// 信号阻塞集 
	//? int sigprocmask(int how, const sigset_t* set, sigset_t* oldset)
	//! 代码
	//sigset_t set;
	//sigset_t old_set;
	//

	//signal(SIGQUIT, handler1);

	//// 添加编号为SIGQUIT的信号
	//sigaddset(&set, SIGQUIT);
	//sigemptyset(&set);
	//sigemptyset(&old_set);
	//sigaddset(&set, SIGQUIT);
	//

	//// 蒙蔽编号为SIGQUIT的信号
	//std::cout << "按下任意键阻塞信号SIGQUIT" << std::endl;
	//getchar(); // 按下任意键
	//int ret = sigprocmask(SIG_BLOCK, &set, &old_set);
	//

	//std::cout << "按下任意键解除阻塞信号SIGQUIT" << s td::endl;
	//getchar(); // 按下任意键
	//int ret2 = sigprocmask(SIG_SETMASK, &old_set, nullptr);

	//std::cout << "按下任意键退出" << std::endl;
	//getchar(); // 按下任意键
	//exit(0);
	//? sigaction(int signum, const struct sigaction* act, struct sigaction* oldact);
	//! sigaction代码
	//int ret = -1;
	//struct sigaction act;
	//// 使用旧的信号处理函数
	//act.sa_handler = handler1;
	//act.sa_flags = 0; // 旧处理函数对应flag == 0

	//ret = sigaction(SIGQUIT, &act, nullptr);
	//if (ret == -1)
	//{
	//	perror("sigaction");
	//	return 1;
	//}
	//while (1)
	//{
	//	sleep(1);
	//}

	//printf("按下任意键退出\n");
	//getchar();
	//---------------------------------------------------------------------------------------------------


	//---------------------------------------------------------------------------------------------------
	//? 使用信号避免僵尸进程
	//! 代码
	//pid_t pid = -1;
	//pid = fork();
	//struct sigaction act;
	//act.sa_handler = handler1;
	//act.sa_flags = 0;
	//// 信号注册
	//sigaction(SIGCHLD, &act, nullptr);
	//for (int i = 0; i < 10; i++)
	//{
	//	pid = fork();
	//	if (pid < 0)
	//	{
	//		std::cout << "fork wrong" << std::endl;
	//		return 1;
	//	}
	//	else if (pid == 0)
	//	{
	//		sleep(i+2);
	//		exit(0);
	//	}
	//}
	//while (1) {
	//	sleep(1);
	//	std::cout << "parents process doing sth\n";
	//}
	//---------------------------------------------------------------------------------------------------

	

	//---------------------------------------------------------------------------------------------------
	//
	//? 计时器： settimer
	//
	//int ret = -1;
	//struct itimerval tmo;

	////第一次触发时间
	//tmo.it_value.tv_sec = 3;
	//tmo.it_value.tv_usec = 0;

	////触发间隔
	//tmo.it_interval.tv_sec = 2;
	//tmo.it_interval.tv_usec = 0;

	////捕捉信号 SIGALRM
	//signal(SIGALRM, handler1);

	////设置定时器
	//ret = setitimer(ITIMER_REAL, &tmo, nullptr);
	//if (-1 == ret)
	//{
	//	perror("setitimer");
	//}

	//---------------------------------------------------------------------------------------------------
   
	
	//---------------------------------------------------------------------------------------------------
    //? ip转换 int inet_pton(AF_INET, char* src, char* dst);
    //?        int inet_ntop(AF_INET, CHAR* src, char* dst, socklen_t size);

    /*char src[] = "192.168.1.1";
    unsigned int num_ip;
    int ret = inet_pton(AF_INET, src, &num_ip);
    assert(ret == 1);
    unsigned char* p = (unsigned char*)&num_ip;
    printf("%d %d %d %d", *p, *(p + 1), *(p + 2), *(p + 3));
    char p_ip[16]= "";*/
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------------------
    /*
    * ipv4套接字结构体。 struct sockaddr_in sock_add;
    * 通用套接字结构体。   struct sockaddr add;
    * int send(char* buf, struct sockaddr*)
    */
    //---------------------------------------------------------------------------------------------------
    // 
    // 
    //---------------------------------------------------------------------------------------------------
    // tcp: 超时重传， 每次收到数据发ACK
    // #include <sys/socket.h>
    //? 创建套接字:int socket(int domain, int type, int protocol) e.g. (AF_INET, SOCK_STREAM, 0);
    //? 连接服务器:int connect(int sockfd, const struct sockaddr*, socklen_t size);
    //! 客户端代码
    //int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //struct sockaddr_in sock_addr;
    //sock_addr.sin_family = AF_INET;
    //sock_addr.sin_port = htons(8080);
    //inet_pton(AF_INET, "192.168.0.105", &sock_addr.sin_addr.s_addr);
    //int connectfd = connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
    //// 读写数据
    //char buf[1024];
    //// char recv_buf[1024];
    //while (1)
    //{
    //    int n = read(STDIN_FILENO, buf, sizeof(buf));
    //    write(sockfd, buf, n);
    //    n = read(sockfd, buf, sizeof(buf));
    //    write(STDOUT_FILENO, buf, n);
    //}
    //close(sockfd);
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------------------
    // 单进程服务器代码
    /*
    * 1. socket(); 
    * 2. in ret = bind(sockfd, const struct sockaddr*, socklen_t size); good--->0  bad--->-1
    * 3. int ret = listen(int sockfd, int backlog);---> listen_socket  int backlog(已完成链接数目+未完成连接数目的最大值)
    * 4. int acceptfd = accept(sockfd, struct sockaddr*, socklen_t* size);---> accept_socket
    *    如果连接队列没有新连接会阻塞。
    */
    //int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //struct sockaddr_in addr;
    //addr.sin_family = AF_INET;
    //addr.sin_port = htons(7111);
    //inet_pton(AF_INET, "192.168.75.159", &addr.sin_addr.s_addr);

    //int bind_ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    //assert(bind_ret >= 0);
    //int lret = listen(sockfd, 128); // 这个位置直接将sockfd--->listenfd。
    //assert(lret >= 0);
    //struct sockaddr_in client_addr;
    //bzero(&client_addr, sizeof(client_addr));
    //socklen_t size = sizeof(struct sockaddr_in);

    //int acceptfd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
    //assert(acceptfd >= 0);
    //std::cout << acceptfd << std::endl;
    //char buf[1024];
    //while (1)
    //{
    //    int m = write(acceptfd, "hello", 5);
    //    // write(acceptfd, "hello", 5);
    //    int n = read(acceptfd, buf, sizeof(buf));
    //    // std::cout << buf << std::endl;
    //    write(STDOUT_FILENO, buf, n);
    //}
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------------------
    /*
    * 粘包：服务器发送过快，前面发送的在缓冲区的数据没来的急读， 后面的数据就来了， 没法确定读取地顺序
    *   解决方法：1. 约定好， 一次发送固定字节数目。
    *            2. 数据结尾加个'\n'
    *            3. 头部加上数据大小。
    */
    //---------------------------------------------------------------------------------------------------


    //---------------------------------------------------------------------------------------------------
    /*
    * TCP: 三次握手过程SYN---->SYN+ACK------>ACK
    * TCP: 报文格式：源端口， 目的端口， 序号， 确认号（序号+标志位+数据字段长度）， 
    *      控制位（ACK, SYN, FIN, URG), 偏移量， 校验和, 窗口尺寸。
    * TCP：四次挥手client：FIN---->ACK 客户端处于半关闭状态，只关闭了应用层（只能收不能发
    *             server：FIN----->ACK 客户端等待2MSL（最大报文生存时间，2-3min） 
    *      为什么要等待2MSL： 第四次ACK可能丢失，若丢失，服务器会重发FIN。 主动关闭连接的会等待2MSL。
    * TCP：滑动窗口: 每一次读取数据之后， 回复TCP报文携带当前缓冲区大小。
    */
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------------------
    /*
    * 多进程服务器实现处理多个客户端
    * fork() 父进程关闭链接套接字（acceptfd），子进程关闭监听套接字（listenfd）
    */
	//sigset_t set;
	//sigemptyset(&set);
	//sigaddset(&set, SIGCHLD);
	//sigprocmask(SIG_BLOCK, &set, nullptr);

 //   int lfd = socket(AF_INET, SOCK_STREAM, 0);
 //   
 //   struct sockaddr_in addr;
 //   addr.sin_family = AF_INET;
 //   addr.sin_port = htons(8888);
 //   inet_pton(AF_INET, "192.168.148.159", &addr.sin_addr.s_addr);
 //   int bind_ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
 //   assert(bind_ret >= 0);
 //   int listen_ret = listen(lfd, 20);
 //   // int lfd = Bind(8888, "192.168.75.159");
 //   // listen(lfd, 10);
 //   struct sockaddr_in cliaddr;
 //   socklen_t size = sizeof(cliaddr);
 //   char buf[1024];
 //   while (1)
 //   {
 //       bzero(&cliaddr, sizeof(cliaddr));
 //       int accfd = accept(lfd, (struct sockaddr*)&cliaddr, &size);
 //       assert(accfd > 0);
 //       pid_t pid;
 //       pid = fork();
 //       assert(pid >= 0);
 //       if (pid == 0)
 //       {
 //           // 子进程 关闭lfd
 //           close(lfd);
 //           // 处理业务, 回射服务器
	//		while (1) {
	//			char bufchild[1024];
	//			int n = read(accfd, buf, sizeof(buf));
	//			assert(n > 0);
	//			write(STDOUT_FILENO, buf, n);
	//			// std::cout << buf << std::endl;
	//			n = read(STDIN_FILENO, buf, sizeof(buf));
	//			write(accfd, buf, n);
	//			//close(accfd);
	//		}
 //       }
 //       if (pid > 0)
 //       {
 //           // 父进程 关闭accfd
 //           close(accfd);
	//		// 回收资源
	//		// 注册信号回调函数
	//		struct sigaction act;
	//		act.sa_flags = 0;
	//		act.sa_handler = free_process;
	//		sigemptyset(&act.sa_mask);
	//		sigaction(SIGCHLD, &act, nullptr);
	//		sigprocmask(SIG_UNBLOCK, &set, nullptr);
 //       }
 //   }
//----------------------------------------------------------------------------------------------------------
//? 多线程
	//! 线程号获取code:
	//-----------------------------------------
	//pthread_t tid = 0;
	//tid = pthread_self();
	//std::cout << tid << std::endl;
	//-----------------------------------------
	//! 判断线程号code:
	//-----------------------------------------
	//pthread_t tid;
	//tid = pthread_self();
	//if (pthread_equal(tid, pthread_self())) {
	//	std::cout << "two thread equals" << std::endl;
	//}
	//-----------------------------------------
	//!线程创建
	//? int pthread_creat(pthread_t* tid, const pthread_attr_t* attr, (void*)(*start_routine)(void*), void* arg);
	//? int pthread_join(pthread_t tid, void**); 默认阻塞
	//! code:
	//auto cb = [](void* arg)->void* {
	//	pthread_t* tid = new pthread_t();
	//	int num = 50;
	//	pthread_create(tid, nullptr, [](void* num)->void* {
	//		int* inum = (int*)num;
	//		while((*inum)--)
	//			printf("int thread2 %d\n", *inum);
	//		}, &num);
	//	std::cout << "create thread" << std::endl;
	//	int* a = (int*)arg;
	//	for (int i = 0; i < *a; i++)
	//	{
	//		std::cout << "in child thread:" << i << std::endl;
	//	}
	//	pthread_join(*tid, nullptr);
	//};
	//pthread_t tid;
	//int a = 20;
	//int ret = pthread_create(&tid, nullptr, cb, &a);
	//for (int i = 0; i < 20; i++)
	//{
	//	printf("int parent thread%d\n", i);
	//}

	//pthread_join(tid, nullptr);
	//-----------------------------------------
	//? int pthread_detach(pthread_t tid); 非阻塞, 进程退出，所有线程都结束。
	//? detach状态的线程不能被join();
	//-----------------------------------------

	//-----------------------------------------
	//? 线程属性 struct pthread_attr_t; 
	//? int pthread_attr_init(pthread_attr_t* attr);
	//? int pthread_attr_destory(pthread_attr_t* attr);
	//? 线程分离状态初始化： int pthread_attr_setdetachstate(pthread_attr_t* attr, int detachstate);
	//? int pthread_attr_getdetachstate(pthread_attr_t* attr, int*detachstate);
	//? 主要成员： 线程分离状态， 线程栈大小， 线程栈警戒缓冲区大小， 线程最低地址。
	//! code
	//pthread_attr_t attr;
	//int retc = pthread_attr_init(&attr);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//assert(retc == 0);
	//// int retd = pthread_attr_destroy(&attr);
	//// assert(retd == 0);

	//pthread_t tid;
	//int a = 20;
	//pthread_create(&tid, &attr, tfunc, &a);
	//int joinret = pthread_join(tid, nullptr);
	//std::cout << "joinret = " << joinret << '\n';
	
	//-----------------------------------------
//----------------------------------------------------------------------------------------------------------
//? C++内存
	// 堆， 栈， 全局数据区（BSS（未初始化），DATA（已初始化）， 常量区， 代码区（存放代码段））
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//? 死锁产生条件
	// 资源互斥， 不可剥夺， 请求和保持， 循环等待
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//? 互斥锁 
	//? int pthread_mutex_init(); int pthread_mutex_lock(); 
	//? int pthread_mutex_unlock(); int pthread_mutex_trylock();
	//----------------------------------------------------------
	//! code
	pthread_t tid1;
	pthread_t tid2;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, nullptr);
	auto fun1 = [](void* m)->void* {
		std::cout << "create thread1\n";
		while (num < 10000) {
			pthread_mutex_lock((pthread_mutex_t*)m);
			printf("in fun1 num = %d\n", num);
			//sleep(1);
			num++;
			pthread_mutex_unlock((pthread_mutex_t*)m);

		}
	};
	auto fun2 = [](void* m)->void* {
		std::cout << "create thread2\n";
		while (num < 10000)
		{
			pthread_mutex_lock((pthread_mutex_t*)m);
			printf("in fun2 num = %d\n", num);
			//sleep(1);
			num++;
			pthread_mutex_unlock((pthread_mutex_t*)m);
		}
	};
	pthread_create(&tid1, nullptr, fun1, &mutex);
	pthread_create(&tid2, nullptr, fun2, &mutex);
	pthread_join(tid1, nullptr);
	pthread_join(tid2, nullptr);
	//----------------------------------------------------------

//----------------------------------------------------------------------------------------------------------

}