#pragma once

#include<cstdio>
#include<unistd.h>

int test_zombie()
{   
	//创建子进程
	pid_t pid = fork();

	if (pid == 0)
		puts("Hi, I am child process");
	else {
		printf("Child Process ID: %d\n", pid);
		sleep(30);
	}
	if (pid == 0)
		puts("End child process");
	else
		puts("End parent process");

	return 0;

}