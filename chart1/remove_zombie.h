#pragma once
#include<cstdio>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/wait.h>

void read_childproc(int sig)
{
	int status;
	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		printf("Remove proc id:%d\n", id);
		printf("Child send: %d\n", WEXITSTATUS(status));
	}
}


int remove_zombie_by_signal()
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if (pid == 0) {
		puts("Hi, i am child process");
		sleep(10);
		return 12;
	}
	else {
		printf("Child pid is: %d\n", pid);
		pid = fork();//创建另一个子进程
		if (pid == 0) {
			puts("Hi, i am another process");
			sleep(10);
			exit(23);
		}
		else {
			int i;
			printf("Child pid is: %d\n", pid);
			for (i = 0; i < 5; i++) {
				puts("wait.....");
				sleep(60);
			}
		
		}
	}
}