#pragma once

#include<cstdio>
#include<unistd.h>
#include<signal.h>


void timeout(int sig)
{
	if (sig == SIGALRM)
		puts("tIME OUT");
	alarm(2);
}

int test_sigaction()
{
	int i;
	struct sigaction act;

	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	//注册信号处理函数
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for (i = 0; i < 3; i++)
	{
		puts("wait.......");
		sleep(100);
	}

	return 0;
}