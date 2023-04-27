#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<signal.h>
#include<netinet/in.h>
#include<fcntl.h>

#define BUF_SIZE 30

void error_handling(char* message);



int peek_recv_server(char* port)
{

	if (port == nullptr) {
		printf("port is null\n");
		exit(1);
	}

	int str_len;
	int acpt_sock;
	int recv_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_size;

	char buf[BUF_SIZE];

	acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (acpt_sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if (bind(acpt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(acpt_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	recv_sock = accept(acpt_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (recv_sock == -1)
		error_handling("accept() error");
	

	while (1) {
		//MSG_PEEK不从缓冲区清除消息，不存在数据也不会阻塞
		//
		str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
		if (str_len > 0)
			break;
	}

	buf[str_len] = 0;
	printf("Buffering %d byte:%s \n", str_len, buf);

	str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
	buf[str_len] = 0;
	printf("Read again:%s \n", buf);

	close(recv_sock);
	close(acpt_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
