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

	//清零+将标准输入描述符加入到监听队列
	FD_ZERO(&reads);
	FD_SET(0, &reads);

	while (true)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		//监听标准输入描述符
		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {//超时没有事件发生
			puts("Time out");
		}
		else {//有事件发生
			if (FD_ISSET(0, &temps)) {
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console:%s", buf);
			}
		}
	}

	return 0;

}