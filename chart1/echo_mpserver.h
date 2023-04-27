#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include<sys/signal.h>
#include<sys/wait.h>

void error_handling(char* message);
void read_childproc(int sig);


#define BUF_SIZE  1024

int echo_msp_server(char* port)
{

	if (port == nullptr) {
		printf("port is null\n");
		exit(1);
	}

	int serv_sock;
	int clnt_sock;
	int str_len = 0;
	int i;

	pid_t pid;
	struct sigaction act;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_size;

	char buf[BUF_SIZE];

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socke() error");

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	int ret = sigaction(SIGCHLD, &act, 0);
	if (ret == -1) {
		error_handling("sigaction error()");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	//父进程接收到消息，fork子进程去处理socket数据的发送
	while (1) {
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected...");

		pid = fork();
		if (pid == -1) {
			printf("fork() error");
			close(clnt_sock);
			continue;
		}


		if (pid == 0) {//子进程区域
			close(serv_sock);
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);

			//子进程由于fork产生了一个clnt_sock，这里只是复制了一个文件描述符
			printf("child proc accept clnt_sock is %d ", clnt_sock);
			close(clnt_sock);
			puts("client disconnected....");
			return 0;
		}
		else 
			//父进程也有这个clnt_sock，也必须进行删除归还给操作系统
			printf("parent proc accept clnt_sock is %d ", clnt_sock);
			close(clnt_sock);
	}

	close(serv_sock);
	return 0;

}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


void read_childproc(int sig)
{
	int status;
	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		printf("Remove proc id:%d\n", id);
		printf("Child send: %d\n", WEXITSTATUS(status));
	}
}