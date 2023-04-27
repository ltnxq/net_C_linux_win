
#pragma once

#include<cstdio>
#include<unistd.h>
#include<signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");
	//2s����һ��SIGALRM�ź�
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

	//�����źŲ�����ʱ�򣬲���ϵͳ�ỽ������sleep�������Ľ���
	//�����Ժ�������
	for (int i = 0; i < 3; i++)
	{
		puts("wait.....");
		sleep(100);
	}

	return 0;
}
