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

/// <summary>
/// 1、服务器端fork子进程专门负责接收消息写入到本地的文件
/// 2、服务器端接收到客户端连接fork子进程接收消息
/// </summary>
/// <param name="port"></param>
/// <returns></returns>

int echo_store_server(char* port)
{

	if (port == nullptr) {
		printf("port is null\n");
		exit(1);
	}

	int serv_sock;
	int clnt_sock;
	int str_len = 0;
	int fds[2];

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

	if (pipe(fds) == -1)
		error_handling("pipe() error");

	pid = fork();
	if (pid == 0) {
		FILE* fp = fopen("echomsg.txt", "wt");
		char msgbuf[BUF_SIZE];
		int i = 0, len = 0;

		for (i = 0; i < 10; i++)
		{
			len = read(fds[0], msgbuf, BUF_SIZE);
			fwrite((void*)msgbuf, 1, len, fp);

		}
		fclose(fp);
		return 0;
	}

	while (1) {
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			continue;
		else
			puts("new Client conected...");

		pid = fork();
		if (pid == 0) {
			close(serv_sock);
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
				write(clnt_sock, buf, str_len);
				write(fds[1], buf, str_len);
			}

			close(clnt_sock);
			puts("client disconnected......");
		}
		else
		{
			close(clnt_sock);
		}

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