#pragma once

#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

void* thread_main(void* arg);

/// <summary>
///由于pthread库不是Linux系统默认的库，连接时需要使用库libpthread.a,所以在使用pthread_create创建线程时，在编译中要加-lpthread参数:
///gcc - o pthread - lpthread pthread.c
/// </summary>
/// <returns></returns>

int threadMain() 
{
	pthread_t t_id;
	int thread_num = 5;

	if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_num) != 0) {
		puts("thread create error()");
		return -1;
	};
	sleep(10);
	puts("end of main");
	return 0;

}

void* thread_main(void* arg)
{
	int i;
	int cnt = *((int*)arg);
	for (i = 0; i < cnt; i++) {
		sleep(1);
		puts("running thread");
	}

	return NULL;
}
