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
int main()
{

    //-------------------------------------------------------------------------
    // 网络字节序转换  htonl()  ntohl()

    /*char* buf = "192.168.1.1";
    int num = *(int*)buf;
    int turned_num = htonl(num);
    buf = (char*)&turned_num;*/
    //-------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // ip转换 int inet_pton(AF_INET, char* src, char* dst);
    //        int inet_ntop(AF_INET, CHAR* src, char* dst, socklen_t size);

    /*char src[] = "192.168.1.1";
    unsigned int num_ip;
    int ret = inet_pton(AF_INET, src, &num_ip);
    assert(ret == 1);
    unsigned char* p = (unsigned char*)&num_ip;
    printf("%d %d %d %d", *p, *(p + 1), *(p + 2), *(p + 3));
    char p_ip[16]= "";*/
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    /*
    * ipv4套接字结构体。 struct sockaddr_in sock_add;
    * 通用套接字结构体。   struct sockaddr add;
    * int send(char* buf, struct sockaddr*)
    */
    //-------------------------------------------------------------------------
    // 
    // 
    //-------------------------------------------------------------------------
    /*
    * tcp: 超时重传， 每次收到数据发ACK
    * #include <sys/socket.h>
    * 创建套接字： int socket(int domain, int type, int protocol) e.g. (AF_INET, SOCK_STREAM, 0);
    * 连接服务器： int connect(int sockfd, const struct sockaddr*, socklen_t size);
    * 
    */
    //// 客户端代码
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
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    // 服务器代码
    /*
    * 1. socket(); 
    * 2. in ret = bind(sockfd, const struct sockaddr*, socklen_t size); good--->0  bad--->-1
    * 3. int ret = listen(int sockfd, int backlog);---> listen_socket  int backlog(已完成链接数目+未完成连接数目的最大值)
    * 4. int acceptfd = accept(sockfd, struct sockaddr*, socklen_t* size);---> accept_socket
    *    如果连接队列没有新连接会阻塞。
    */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7111);
    inet_pton(AF_INET, "192.168.75.159", &addr.sin_addr.s_addr);

    int bind_ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    assert(bind_ret >= 0);
    int lret = listen(sockfd, 128); // 这个位置直接将sockfd--->listenfd。
    assert(lret >= 0);
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    socklen_t size = sizeof(struct sockaddr_in);

    int acceptfd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
    assert(acceptfd >= 0);
    //std::cout << acceptfd << std::endl;
    char buf[1024];
    while (1)
    {
        int m = write(acceptfd, "hello", 5);
        // write(acceptfd, "hello", 5);
        int n = read(acceptfd, buf, sizeof(buf));
        // std::cout << buf << std::endl;
        write(STDOUT_FILENO, buf, n);
    }
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    /*
    * 粘包：服务器发送过快，前面发送的在缓冲区的数据没来的急读， 后面的数据就来了， 没法确定读取地顺序
    *   解决方法：1. 约定好， 一次发送固定字节数目。
    *            2. 数据结尾加个'\n'
    *            3. 头部加上数据大小。
    */
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
    /*
    * TCP:三次握手过程SYN---->SYN+ACK------>ACK
    * TCP:四次挥手过程FIN---->ACK  
    */






    
    



}