#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>

void error_handling(char* message);
void setnonblockingmode(int fd);

#define BUF_SIZE  4


int echo_epoll_server(char* port)
{

	if (port == nullptr) {
		printf("port is null\n");
		exit(1);
	}

	int serv_sock;
	int clnt_sock;
	int str_len = 0;
	int i;
	int fd_num = 0;
	char buf[BUF_SIZE];

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	//epoll_event用于构造事件
	struct epoll_event* events;
	struct epoll_event event;
	int epollfd, event_cnt;


	socklen_t clnt_addr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	//创建epoll例程，将监听socket设置到epoll中
	//要进行malloc内存分配
	epollfd = epoll_create(50);
	events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * 50);
	if (epollfd == -1)
		error_handling("epoll_create() error");
	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_sock, &event) == -1)
		error_handling("epoll ctl add error()");
	int time = 0;


	while (1)
	{
		//1、调用epoll_wait
		//2、判断是server还是client 如果是serversock的话，那么将生成传输数据的sock，然后再加入到epoll中
		//如果是数据传输的sock，那么就进行数据读取
		event_cnt = epoll_wait(epollfd, events, 50, -1);
		if (event_cnt == -1) {
			puts("epoll wait error()");
			break;
		}
		time += 1;
		printf("return epoll wait times-%d\n", time);
		for (i = 0; i < event_cnt; i++) {
			if (events[i].data.fd == serv_sock) {
				//调用accept、建立和客户端通信的套接字
				clnt_addr_size = sizeof(clnt_addr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
				if (clnt_sock == -1)
					error_handling("accept() error");
				//将收发数据的socket设置为非阻碍的
				setnonblockingmode(clnt_sock);
				//改为边沿触发的方式，默认是水平触发
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clnt_sock;
				//将和客户端建立通信的sock加入epoll
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clnt_sock, &event) == -1)
					error_handling("epoll ctl add error()");
				printf("connected client:%d\n", clnt_sock);
			}
			else {
				//读取数据
				str_len = read(events[i].data.fd, buf, BUF_SIZE);
				//close request
				if (str_len == 0) {
					epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					//关闭socket
					close(events[i].data.fd);

					printf("close conneted clinet sock:%d\n", events[i].data.fd);
				}
				else if(strlen < 0)
				{   
					//read => -1
					//srelen < 0 并且errno等于EAGAIN的话，表示没有数据可读
					if (errno == EAGAIN)
						break;
				}
				else {
					write(events[i].data.fd, buf, str_len);//echo!
				}
			}
		}
	}

	close(serv_sock);
	close(epollfd);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void setnonblockingmode(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, flag | O_NONBLOCK);
}
