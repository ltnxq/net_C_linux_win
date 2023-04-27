#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int test_wait() {
	int status;
	pid_t pid = fork();

	if (pid == 0)//�ӽ���ֱ�ӷ���
		return 3;
	else {//������Ҫ�������߼�
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