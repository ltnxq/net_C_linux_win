#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <string.h>

void* thread_main(void* arg);


int threadMain()
{
	pthread_t t_id;
	int thread_num = 5;
	void* thr_ret;

	if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_num) != 0) {
		puts("thread create error()");
		return -1;
	};

	//等待线程运行结束
	if (pthread_join(t_id, &thr_ret) != 0)
	{
		puts("pthread_join() error");
		return -1;
	}

	printf("Thread return message: %s \n", (char*)thr_ret);
	//释放内存
	free(thr_ret);
	
	return 0;

}

void* thread_main(void* arg)
{
	int i;
	int cnt = *((int*)arg);
	char* msg = (char*)malloc(sizeof(char) * 50);
	strcpy(msg, "Hello I am thread~\n");
	for (i = 0; i < cnt; i++) {
		sleep(1);
		puts("running thread");
	}

	return (void*)msg;
}
