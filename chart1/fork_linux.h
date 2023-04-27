#pragma once

#include<stdio.h>
#include<unistd.h>

int gval = 10;

int  fork_test()
{
	pid_t pid;
	int lval = 20;
	gval++, lval += 5;//11 25

	pid = fork();
	if (pid == 0) {//child process
		gval += 2, lval += 2;
	}
	else {//parent process
		gval -= 2, lval -= 2;
	}

	if (pid == 0)
		printf("Child proc:[%d,%d]\n", gval, lval);
	else
		printf("Parent proc:[%d,%d]\n", gval, lval);

	return 0;

}