#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* messae);

int news_recv_brd_server( char* port)
{

	if (port == nullptr ) {
		printf(" port is null\n");
		exit(1);
	}

	int recv_sock;
	int clnt_sock;
	int str_len = 0;
	int i;

	struct sockaddr_in serv_addr;
	struct ip_mreq join_adr;
	socklen_t clnt_addr_size;

	char buf[BUF_SIZE];

	//SOCK_DGRAM  代表是UDP协议  注意编程时候的区分
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (recv_sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if (bind(recv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	while (1)
	{
		str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (str_len < 0)
			break;
		buf[str_len] = 0;
		fputs(buf, stdout);
	}

	close(recv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
