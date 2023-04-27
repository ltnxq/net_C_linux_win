#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int test_wait() {
	int status;
	pid_t pid = fork();

	if (pid == 0)//子进程直接返回
		return 3;
	else {//父进程要处理的逻辑
		printf("Chlid PID:%d \n", pid);
		pid = fork();
		if (pid == 0)
			exit(7);
		else {
			printf("Child PID:%d\n", pid);
			wait(&status);
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			wait(&status);

			wait(&status);
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			sleep(30);
		}
	}

	return 0;
}
