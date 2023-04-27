#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(char* messae);

int news_recv_server(char* multiIp,char* port)
{

	if (port == nullptr || multiIp == nullptr) {
		printf("port is null\n");
		exit(1);
	}

	int serv_sock;
	int clnt_sock;
	int str_len = 0;
	int i;

	struct sockaddr_in serv_addr;
	struct ip_mreq join_adr;
	socklen_t clnt_addr_size;

	char buf[BUF_SIZE];

	//SOCK_DGRAM  代表是UDP协议  注意编程时候的区分
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	join_adr.imr_multiaddr.s_addr = inet_addr(multiIp);
	join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(serv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

	while (1)
	{
		str_len = recvfrom(serv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (str_len < 0)
			break;
		buf[str_len] = 0;
		fputs(buf, stdout);
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
