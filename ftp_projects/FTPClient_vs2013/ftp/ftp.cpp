// ftp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <cstdio>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#include <string>

using namespace std;

#ifndef FTPAPI_H_INCLUDED
#define FTPAPI_H_INCLUDED
#endif

#ifndef FTPRESPONSECODE_H_INCLUDED
#define FTPRESPONSECODE_H_INCLUDED


#define FTP_SUCCESS 200  //成功
#define FTP_SERVICE_READY 220 //服务器就绪
#define FTP_LOGIN_SUCCESS 230 //登录因特网服务器
#define FTP_FILE_ACTION_COMPLETE 250 //文件行为完成
#define FTP_FILE_CREATED 257 //文件创建成功
#define FTP_PASSWORD_REQUIREd 331 //要求密码
#define FTP_LOGIN_PASSWORD_INCORRECT 530 //用户密码错误
#define BUFSIZE 100


#endif // FTPRESPONSECODE_H_INCLUDED

static int filesize;

SOCKET socket_connect(char *host, int port);
SOCKET connect_server(char *host, int port);
int ftp_sendcmd_re(SOCKET sock, char *cmd, char *re_buf, size_t *len);
int ftp_sendcmd(SOCKET sock, char *cmd);
int login_server(SOCKET sock, char *user, char *pwd);
void socket_close(int c_sock);

SOCKET ftp_connect(char *host, int port, char *user, char *pwd); //连接到服务器
int ftp_quit(SOCKET sock); //断开连接
int ftp_type(SOCKET sock, char mode); //设置FTP传输类型
int ftp_cwd(SOCKET sock, char *path); //更改工作目录
int ftp_cdup(SOCKET sock); //回到上级目录
int ftp_mkd(SOCKET sock, char *path); //创建目录
SOCKET ftp_pasv_connect(SOCKET c_sock); //连接到PASV接口
int ftp_list(SOCKET c_sock, char *path, char **data, int *data_len); //列出FTP工作空间的所有目录
int ftp_deletefolder(SOCKET sock, char *path); //删除目录
int ftp_deletefile(SOCKET sock, char *filename); //删除文件
int ftp_renamefile(SOCKET sock, char *s, char *d); //修改文件/目录&移动文件/目录
int ftp_server2local(SOCKET c_sock, char *s, char *d, int * size); //从服务器复制文件到本地 RETR
int ftp_local2server(SOCKET c_sock, char *s, char *d, int * size); //从本地复制文件到服务器 STOR
int ftp_recv(SOCKET sock, char *re_buf, size_t *len); //获取响应码



/**
 * 作用: 创建一个Socket并返回.
 * 参数: IP或域名, 端口
 * 返回值: Socket套接字
 * */
SOCKET socket_connect(char *host, int port)
{
	printf("socket_connect enter.\n");
	int i = 0;
	//初始化 Socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 0);
	if (WSAStartup(socketVersion, &wsaData))
	{
		printf("Init socket dll error!");
		exit(1);
	}

	struct addrinfo hints;
	struct addrinfo *res,*cur;
	int ret1;
	struct sockaddr_in *addr;
	char m_ipaddr[16];
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;     /* Allow IPv4 */
	hints.ai_flags = AI_PASSIVE;/* For wildcard IP address */
	hints.ai_protocol = 0;         /* Any protocol */
	hints.ai_socktype = SOCK_STREAM;

	ret1 = getaddrinfo(host,NULL, &hints, &res);
	if (ret1 == -1) {
		perror("socket_connect() getaddrinfo");
		exit(1);
	}
	for (cur = res; cur != NULL; cur = cur->ai_next) {
		addr = (struct sockaddr_in *)cur->ai_addr;
		sprintf_s(m_ipaddr, "%d.%d.%d.%d",
			(*addr).sin_addr.S_un.S_un_b.s_b1,
			(*addr).sin_addr.S_un.S_un_b.s_b2,
			(*addr).sin_addr.S_un.S_un_b.s_b3,
			(*addr).sin_addr.S_un.S_un_b.s_b4);
		printf("socket_connect m_ipaddr  = %s\n", m_ipaddr);
	}

	//struct hostent * server = gethostbyname(host);
	//if (!server)
	//	return -1;
	//unsigned char ch[4];
	//char ip[20];
	////一个hostname 可以对应多个ip
	//while (server->h_addr_list[i] != NULL)
	//{
	//	memcpy(&ch, server->h_addr_list[i], 4);
	//	sprintf_s(ip, "%d.%d.%d.%d", ch[0], ch[1], ch[2], ch[3]);
	//	//printf("%s\n",ip);
	//	i++;
	//}

	//创建Socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0); //TCP socket
	if (SOCKET_ERROR == s)
	{
		printf("socket_connect,socket_connect,Create Socket Error!");
		exit(1);
	}
	//设置超时连接
	int timeout = 3000; //复杂的网络环境要设置超时判断
	int ret = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
	ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	//指定服务器地址
	struct in_addr dst;
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	int ret2= inet_pton(AF_INET, m_ipaddr, &dst);

	address.sin_addr.S_un = dst.S_un;
	address.sin_port = htons((unsigned short)port);
	//连接
	if (SOCKET_ERROR == connect(s, (LPSOCKADDR)&address, sizeof(address)))
	{
		printf("socket_connect,Can Not Connect To Server IP!\n");
		exit(1);
	}
	return s;
}

/**
 * 作用: 连接到一个FTP服务器，返回socket
 * 参数: IP或域名, 端口
 * 返回值: Socket套接字
 * */
SOCKET connect_server(char *host, int port)
{
	printf("connect_server enter.\n");
	SOCKET ctrl_sock;
	char buf[BUFSIZE];
	int result;
	int len;

	ctrl_sock = socket_connect(host, port);
	if (-1 == ctrl_sock)
	{
		return -1;
	}
	while ((len = recv(ctrl_sock, buf, BUFSIZE, 0)) > 0)
	{
		//len = recv(ctrl_sock, buf, BUFSIZE, 0);
		buf[len] = 0;
		printf("connect_server %s\n", buf); 
	}
	sscanf_s(buf, "%d", &result);

	if (FTP_SERVICE_READY != result)
	{
		printf("FTP Not ready, Close the socet.");
		closesocket(ctrl_sock); //关闭Socket
		return -1;
	}
	return ctrl_sock;
}

/**
 * 作用: send发送命令，并返回recv结果
 * 参数: SOCKET，命令，命令返回码-命令返回描述，命令返回字节数
 * 返回值: 0 表示发送成功  -1表示发送失败
 * */
int ftp_sendcmd_re(SOCKET sock, char *cmd, char *re_buf, size_t *len)
{
	char buf[BUFSIZE];
	int r_len;
	if (send(sock, cmd, strlen(cmd), 0) == -1)
	{
		return -1;
	}
	r_len = recv(sock, buf, BUFSIZE, 0);
	if (r_len < 1)
		return -1;
	buf[r_len] = 0;
	if (NULL != len)
		*len = r_len;
	if (NULL != re_buf)
		strcpy(re_buf,buf);
		//sprintf_s(re_buf, "%s", buf);
	return 0;
}

/**
 * 作用: send发送命令
 * 参数: SOCKET,命令
 * 返回值: FTP响应码
 * */
int ftp_sendcmd(SOCKET sock, char *cmd)
{
	char buf[BUFSIZE];
	int result;
	size_t len;
	printf("FTP Client: %s", cmd);
	result = ftp_sendcmd_re(sock, cmd, buf, &len);
	printf("FTP Server: %s", buf);
	if (cmd[0] == 'R'&&cmd[1] == 'E'&&cmd[2] == 'T'&&cmd[3] == 'R') {
		string tmp = string(buf);
		int start = tmp.find_first_of('(');
		istringstream is(tmp.substr(start+1));
		is >> filesize;
	}
	if (0 == result)
	{
		sscanf_s(buf, "%d", &result);
	}
	return result;
}

/**
 * 作用: 登录FTP服务器
 * 参数: SOCKET套接字，明文用户名，明文密码
 * 返回值: 0 表示登录成功   -1 表示登录失败
 * */
int login_server(SOCKET sock, char *user, char *pwd)
{
	char buf[BUFSIZE];
	int result;
	sprintf_s(buf, "USER %s\r\n", user);
	//这里要对socket进行阻塞
	int timeout = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	result = ftp_sendcmd(sock, buf);
	if (FTP_LOGIN_SUCCESS == result) //直接登录
		return 0;
	else if (FTP_PASSWORD_REQUIREd == result) //需要密码
	{
		sprintf_s(buf, "PASS %s\r\n", pwd);
		result = ftp_sendcmd(sock, buf);
		if (FTP_LOGIN_SUCCESS == result)
		{
			return 0;
		}
		else //530 密码错误
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

/**
 * 作用: winsock使用后，要调用WSACleanup函数关闭网络设备，以便释放其占用的资源
 * 参数: SOCKET
 * 返回值: 无
 * */
void socket_close(int c_sock)
{
	WSACleanup();
}

/**
 * 作用: 连接到FTP服务器
 * 参数: hostname或IP，端口，用户名，密码
 * 返回值: 已连接到FTP服务器的SOCKET   -1 表示登录失败
 * */
SOCKET ftp_connect(char *host, int port, char *user, char *pwd)
{
	printf("ftp_connect enter.\n");
	SOCKET sock;
	sock = connect_server(host, port);
	if (-1 == sock)
	{
		return -1;
	}
	if (-1 == login_server(sock, user, pwd))
	{
		closesocket(sock);
		return -1;
	}
	return sock;
}

/**
 * 作用: 断开FTP服务器
 * 参数: SOCKET
 * 返回值: 成功断开状态码
 * */
int ftp_quit(SOCKET sock)
{
	int result = 0;
	char * tmp = const_cast<char*>("QUIT\r\n");
	result = ftp_sendcmd(sock, tmp);
	closesocket(sock);
	socket_close(sock);
	return result;
}

/**
 * 作用: 设置FTP传输类型 A:ascii I:Binary
 * 参数: SOCkET，类型
 * 返回值: 0 表示成功   -1 表示失败
 * */
int ftp_type(SOCKET sock, char mode)
{
	char buf[BUFSIZ];
	sprintf_s(buf, "TYPE %c\r\n", mode);
	if (FTP_SUCCESS != ftp_sendcmd(sock, buf))
		return -1;
	else
		return 0;
}

/**
 * 作用: 更改工作目录
 * 参数: SOCKET，工作目录
 * 返回值: 0 表示成功  -1 表示失败
 * */
int ftp_cwd(SOCKET sock, char *path)
{
	char buf[BUFSIZE];
	int result;
	sprintf_s(buf, "CWD %s\r\n", path);
	result = ftp_sendcmd(sock, buf);
	if (FTP_FILE_ACTION_COMPLETE != result)  //250 文件行为完成
		return -1;
	else
		return 0;
}

/**
 * 作用: 回到上级目录
 * 参数: SOCKET
 * 返回值: 0 正常操作返回  result 服务器返回响应码
 * */
int ftp_cdup(SOCKET sock)
{
	int result;
	char * tmp = const_cast<char*>("CDUP\r\n");
	result = ftp_sendcmd(sock, tmp);

	if (FTP_FILE_ACTION_COMPLETE == result || FTP_SUCCESS == result)
		return 0;
	else
		return result;
}

/**
 * 作用: 创建目录
 * 参数: SOCKET，文件目录路径(可相对路径，绝对路径)
 * 返回值: 0 正常操作返回  result 服务器返回响应码
 * */
int ftp_mkd(SOCKET sock, char *path)
{
	char buf[BUFSIZE];
	int result;
	sprintf_s(buf, "MKD %s\r\n", path);
	result = ftp_sendcmd(sock, buf);
	if (FTP_FILE_CREATED != result) //257 路径名建立
		return result; //550 目录已存在
	else
		return 0;
}

/**
 * 作用: 连接到PASV接口
 *       PASV（被动）方式的连接过程是：
 *       客户端向服务器的FTP端口（默认是21）发送连接请求，
 *       服务器接受连接，建立一条命令链路。
 * 参数: 命令链路SOCKET cmd-socket
 * 返回值: 数据链路SOCKET raw-socket  -1 表示创建失败
 * */
SOCKET ftp_pasv_connect(SOCKET c_sock)
{
	SOCKET r_sock;
	int send_result;
	size_t len;
	int addr[6]; //IP*4+Port*2
	char buf[BUFSIZE];
	char result_buf[BUFSIZE];

	//设置PASV被动模式
	memset(buf, sizeof(buf), 0);
	sprintf_s(buf, "PASV\r\n");
	send_result = ftp_sendcmd_re(c_sock, buf, result_buf, &len);
	if (send_result == 0)
	{
		sscanf(result_buf, "%*[^(](%d,%d,%d,%d,%d,%d)",
			&addr[0], &addr[1], &addr[2], &addr[3],
			&addr[4], &addr[5]);
	}

	//连接PASV端口
	memset(buf, sizeof(buf), 0);
	sprintf_s(buf, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
	r_sock = socket_connect(buf, addr[4] * 256 + addr[5]);
	if (-1 == r_sock)
		return -1;
	return r_sock;
}

/**
 * 作用: 列出FTP工作空间的所有目录
 * 参数: 命令链路SOCKET，工作空间，列表信息，列表信息大小
 * 返回值: 0 表示列表成功  result>0 表示其他错误响应码  -1 表示创建pasv错误
 * */
int ftp_list(SOCKET c_sock, char *path, char **data, int *data_len)
{
	SOCKET r_sock;
	char buf[BUFSIZE];
	int send_re;
	int result;
	int len, buf_len, total_len;

	//连接到PASV接口
	r_sock = ftp_pasv_connect(c_sock);
	if (-1 == r_sock)
	{
		return -1;
	}
	//发送LIST命令
	memset(buf, sizeof(buf), 0);
	sprintf_s(buf, "LIST %s\r\n", path);
	send_re = ftp_sendcmd(c_sock, buf);
	if (send_re >= 300 || send_re == 0)
		return send_re;
	len = total_len = 0;
	buf_len = BUFSIZE;
	char *re_buf = (char *)malloc(buf_len);
	while ((len = recv(r_sock, buf, BUFSIZE, 0)) > 0)
	{
		if (total_len + len > buf_len)
		{
			buf_len *= 2;
			char *re_buf_n = (char *)malloc(buf_len);
			memcpy(re_buf_n, re_buf, total_len);
			free(re_buf);
			re_buf = re_buf_n;
		}
		memcpy(re_buf + total_len, buf, len);
		total_len += len;
	}
	closesocket(r_sock);
	*data = re_buf;
	*data_len = total_len;
	return 0;
}

/**
 * 作用: 删除目录
 * 参数: 命令链路SOCKET，路径目录
 * 返回值: 0 表示列表成功  result>0 表示其他错误响应码
 * */
int ftp_deletefolder(SOCKET sock, char *path)
{
	char buf[BUFSIZE];
	int result;
	sprintf_s(buf, "RMD %s\r\n", path);
	result = ftp_sendcmd(sock, buf);
	if (FTP_FILE_ACTION_COMPLETE != result)
	{
		//550 Directory not empty.
		//550 Directory not found.
		return result;
	}
	return 0;
}

/**
 * 作用: 删除文件
 * 参数: 命令链路SOCKET，路径文件(相对/绝对)
 * 返回值: 0 表示列表成功  result>0 表示其他错误响应码
 * */
int ftp_deletefile(SOCKET sock, char *filename)
{
	char buf[BUFSIZE];
	int result;
	sprintf_s(buf, "DELE %s\r\n", filename);
	result = ftp_sendcmd(sock, buf);
	if (FTP_FILE_ACTION_COMPLETE != 250) //250 File deleted successfully
	{
		//550 File not found.
		return result;
	}
	return 0;
}

/**
 * 作用: 修改文件名&移动目录
 * 参数: 命令链路SOCKET，源地址，目的地址
 * 返回值: 0 表示列表成功  result>0 表示其他错误响应码
 * */
int ftp_renamefile(SOCKET sock, char *s, char *d)
{
	char buf[BUFSIZE];
	int result;
	sprintf_s(buf, "RNFR %s\r\n", s);
	result = ftp_sendcmd(sock, buf);
	if (350 != result) //350 文件行为暂停，因为要进行移动操作
		return result;
	sprintf_s(buf, "RNTO %s\r\n", d);
	result = ftp_sendcmd(sock, buf);
	if (FTP_FILE_ACTION_COMPLETE != result)
	{
		return result;
	}
	return 0;
}

/**
 * 作用: 从服务器复制文件到本地 RETR
 * 参数: SOCKET，源地址，目的地址，文件大小
 * 返回值: 0 表示列表成功  result>0 表示其他错误响应码
 *         -1:文件创建失败  -2 pasv接口错误
 * */
int ftp_server2local(SOCKET c_sock, char *s, char *d, int * size)
{
	SOCKET d_sock;
	int len, write_len;
	char buf[BUFSIZ];
	int result;
	*size = 0;
	//打开本地文件
	FILE * fp = fopen(d, "wb");
	if (NULL == fp)
	{
		printf("Can't Open the file.\n");
		return -1;
	}
	//设置传输模式
	ftp_type(c_sock, 'I');

	//连接到PASV接口 用于传输文件
	d_sock = ftp_pasv_connect(c_sock);
	if (-1 == d_sock)
	{
		fclose(fp); //关闭文件
		return -2;
	}

	//发送RETR命令
	memset(buf, sizeof(buf), 0);
	sprintf_s(buf, "RETR %s\r\n", s);
	result = ftp_sendcmd(c_sock, buf);
	if (result >= 300 || result == 0) //失败可能是没有权限什么的，具体看响应码
	{
		fclose(fp);
		return result;
	}
	//开始向PASV读取数据(下载)
	memset(buf, sizeof(buf), 0);
	while ((len = recv(d_sock, buf, BUFSIZE, 0)) > 0)
	{
		write_len = fwrite(&buf, len, 1, fp);
		if (write_len != 1) //写入文件不完整
		{
			closesocket(d_sock); //关闭套接字
			fclose(fp); //关闭文件
			return -1;
		}
		if (filesize != 0) {
			printf(" %.3f%%\n", (*size / (float) filesize) * 100);
		}

		if (NULL != size)
		{
			*size += BUFSIZE;
		}
	}
	//下载完成
	closesocket(d_sock);
	fclose(fp);
	return 0;
}

/**
 * 作用: 从本地复制文件到服务器 STOR
 * 参数: SOCKET，源地址，目的地址，文件大小
 * 返回值: 0 表示列表成功  result>0 表示其他错误响应码
 *         -1:文件创建失败  -2 pasv接口错误
 * */
int ftp_local2server(SOCKET c_sock, char *s, char *d, int * size)
{
	static int handled = 0;
	SOCKET d_sock;
	size_t len, send_len;
	char buf[BUFSIZE];
	FILE * fp;
	int send_re;
	int result;
	//打开本地文件
	fp = fopen(s, "rb");
	fseek(fp,0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (NULL == fp)
	{
		printf("Can't Not Open the file.\n");
		return -1;
	}
	//设置传输模式
	ftp_type(c_sock, 'I');
	//连接到PASV接口
	d_sock = ftp_pasv_connect(c_sock);
	if (d_sock == -1)
	{
		fclose(fp);
		return -1;
	}

	//发送STOR命令
	memset(buf, sizeof(buf), 0);
	sprintf_s(buf, "STOR %s\r\n", d);
	send_re = ftp_sendcmd(c_sock, buf);
	if (send_re >= 300 || send_re == 0)
	{
		fclose(fp);
		return send_re;
	}

	//开始向PASV通道写数据
	memset(buf, sizeof(buf), 0);
	while ((len = fread(buf, 1, BUFSIZE, fp)) > 0)
	{
		send_len = send(d_sock, buf, len, 0);
		if (send_len != len)
		{
			closesocket(d_sock);
			fclose(fp);
			return -1;
		}
		printf(" %.3f%%\n", (handled / (float)length) * 100);
		handled += BUFSIZE;
		if (NULL != size)
		{
			*size += BUFSIZE;
		}
	}
	handled = 0;
	//完成上传
	closesocket(d_sock);
	fclose(fp);

	//向服务器接收响应码
	memset(buf, sizeof(buf), 0);
	len = recv(c_sock, buf, BUFSIZE, 0);
	buf[len] = 0;
	sscanf_s(buf, "%d", &result);
	if (result >= 300)
	{
		return result;
	}
	return 0;
}

int main()
{
	SOCKET s;
	string cmd;
	string arg1,arg2,arg3,arg4;
	while (true) {
		cout << "FTP>";
		cin >> cmd;
		if (cmd == "connect") {
			cin >> arg1 >> arg2 >> arg3 >> arg4;
			char *host = (char *)(arg1.c_str());
			char *port= (char *)(arg2.c_str());
			char *user = (char *)(arg3.c_str());
			char *pwd = (char *)(arg4.c_str());
			s = ftp_connect(host, 21, user, pwd);
			cout << "\n\n";
			if (-1 == s)
				cout << "status:-1\n";
			else cout << "status:0\n";
			continue;
		}
		else if (cmd == "quit") {
			int status=ftp_quit(s);
			cout << "\n\n";
			cout << "status:" << status << "\n";
			break;
		}
		else if (cmd == "mkd") {
			cin >> arg1;
			char *path= (char *)(arg1.c_str());
			int status=ftp_mkd(s, path);
			cout << "\n\n";
			cout << "status:" << status << "\n";

		}else if(cmd=="list") {
			cin >> arg1;
			char *path = (char *)(arg1.c_str());
			char *data = (char *)malloc(BUFSIZE);
			int data_len;
			int status=ftp_list(s, path, &data, &data_len);
			data[data_len] = 0;
			cout << data;
			delete data;
			cout << "\n\n";
			if (0 == status)
				cout << "status:0\n";
			else cout << "status:-1\n";

		}
		else if (cmd == "delfolder") {
			cin >> arg1;
			char *path = (char *)(arg1.c_str());
			int status=ftp_deletefolder(s, path);
			cout << "\n\n";
			cout << "status:" << status << "\n";
		}
		else if (cmd == "delfile") {
			cin >> arg1;
			char *path = (char *)(arg1.c_str());
			int status =ftp_deletefile(s, path);
			cout << "\n\n";
			cout << "status:" << status << "\n";
		}
		else if (cmd == "rename") {
			cin >> arg1 >> arg2;
			char *oldname = (char *)(arg1.c_str());
			char *newname = (char *)(arg2.c_str());
			int status=ftp_renamefile(s, oldname, newname);
			cout << "\n\n";
			cout << "status:" << status << "\n";
		}
		else if (cmd == "server2local") {
			cin >> arg1 >> arg2 ;
			char *source = (char *)(arg1.c_str());
			char *dst = (char *)(arg2.c_str());
			int size;
			int status = ftp_server2local(s,source,dst,&size);
			cout << "\n\n";
			cout << "status:" << status << "\n";
		}
		else if (cmd == "local2lserver") {
			cin >> arg1 >> arg2;
			char *source = (char *)(arg1.c_str());
			char *dst = (char *)(arg2.c_str());
			int size;
			int status = ftp_local2server(s, source, dst, &size);
			cout << "\n\n";
			cout << "status:" << status << "\n";
		}
	}
}

