#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>


void error_handling(char* message);

#define BUF_SIZE  1024

int echo_select_server(char* port)
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

	fd_set reads, cpy_reads;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	struct timeval timeout;

	socklen_t clnt_addr_size;

	char message[BUF_SIZE];

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

	FD_ZERO(&reads);
	//1�����ȣ�server_sock�϶�Ҫ����select��fdset��

	FD_SET(serv_sock, &reads);
	int fd_max = serv_sock;
	while (1)
	{   
		//ÿ��ѭ��Ҫ����һ��reads�Ŀ�������Ϊѭ����Ҫ��reads���иı�
		//������select ���û���¼���������ô��Ӧ���ļ�������λ��������Ϊ0
		//reads�൱�ڶ��ļ�����������һЩ����(���ӻ���ɾ��)
		cpy_reads = reads;
		//ÿ��ѭ����ʼ��timeout
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		//����ע����fd_max+1
		fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout);
		if (fd_num == -1)
			break;
		if (fd_num == 0)
			continue;
		
		//������߼���ʾ��ĳһ����������read�¼�����
		for (i = 0; i < fd_max + 1; i++) {
			if (FD_ISSET(i, &cpy_reads)) {
				//����ˣ�˵�����µ����ӽ���
				if (i == serv_sock) {
					clnt_addr_size = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
					//���뵽select�ļ���������
					FD_SET(clnt_sock,&reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;

					printf("Connected client is: %d\n ", clnt_sock);
				}
				else {
					//read message,˵���������������������server_sock  �������Ƕ�Ӧ��clnt_sock
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0) {//˵���Է��Ѿ��ر�
						FD_CLR(i, &reads);
						close(i);
						printf("closed client : %d\n", i);
					}
					else {
						write(i, buf, str_len);  //echo
					}

				}
			}
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
