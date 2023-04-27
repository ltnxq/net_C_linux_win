
#pragma once

#include<cstdio>
#include<unistd.h>
#include<signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");
	//2s产生一个SIGALRM信号
	alarm(2);
}

void keycontrol(int sig)
{
	if (sig == SIGINT)
		puts("ctrl+c pressed");
}

int test_signal()
{
	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);

	//由于信号产生的时候，操作系统会唤醒由于sleep而阻塞的进程
	//唤醒以后不再阻塞
	for (int i = 0; i < 3; i++)
	{
		puts("wait.....");
		sleep(100);
	}

	return 0;
}
