#pragma once
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>
#define BUF_SIZE 30

int test_select()
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	//����+����׼�������������뵽��������
	FD_ZERO(&reads);
	FD_SET(0, &reads);

	while (true)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		//������׼����������
		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {//��ʱû���¼�����
			puts("Time out");
		}
		else {//���¼�����
			if (FD_ISSET(0, &temps)) {
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console:%s", buf);
			}
		}
	}

	return 0;

}