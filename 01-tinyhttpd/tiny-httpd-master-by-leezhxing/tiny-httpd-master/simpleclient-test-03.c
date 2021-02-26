/*
1.request buf::char buf[] = "GET / HTTP/1.1\n\n\n";
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> //自己加的,exit函数需要

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
	char buf[] = "GET / HTTP/1.1\n\n\n";
	char get[1024] = {0};

    //申请一个流 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //填充地址结构，指定服务器的 IP 和 端口
    address.sin_family = AF_INET;
    //inet_addr 可以参考 man inet_addr
    //可以用现代的inet_pton()替代inet_addr(), example 中有参考例子
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8008);/*tinyhttpd的端口*/
    len = sizeof(address);

    //下面的语句可以输出连接的 IP 地址
    //但是inet_ntoa()是过时的方法，应该改用 inet_ntop(可参考 example)。但很多代码仍然遗留着inet_ntoa.
    //printf("%s\n", inet_ntoa( address.sin_addr));

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("oops: client1");
        exit(1);
    }

    /*发送请求*/
    write(sockfd, buf, sizeof(buf));
    /*接收返回数据*/
    read(sockfd,get, sizeof(get));
	/*打印返回数据*/
	printf("-----------------------------show buffer -----------------------------\n");
    printf("get from server = %s\n", get);
	printf("----------------------------------------------------------------------\n");
    close(sockfd);
    exit(0);
}
