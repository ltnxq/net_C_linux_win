#pragma once

#include<unistd.h>
#include<stdio.h>

#define BUF_SIZE  30

int test_pipe1()
{
	int fds[2];
	char str[] = "who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		//�ӽ��̷�����Ϣ �������̶�ȡ 
		write(fds[1], str, sizeof(str));
	}
	else {
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}

	return 0;
}

/// <summary>
/// ʾ��ȫ˫��ͨ�ţ�����pipe��
/// </summary>
/// <returns></returns>
int test_pipe2()
{
	int fds[2];
	char str[] = "who are you?";
	char str2[] = "thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);

	pid = fork();
	if (pid == 0)
	{
		//�ӽ��̷�����Ϣ �������̶�ȡ 
		write(fds[1], str, sizeof(str));

		//��ͣ2��
		sleep(2);
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output:%s\n", buf);
	}
	else {
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc receive:%s\n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}

/// <summary>
/// ˫pipe���˫��ͨ��
/// </summary>
/// <returns></returns>

int test_pipe3()
{
	int fds1[2], fds2[2];
	char str[] = "who are you?";
	char str2[] = "thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	//�½�����pipe
	pipe(fds1);
	pipe(fds2);

	pid = fork();
	if (pid == 0)
	{
		//�ӽ���fds1������Ϣ �������̶�ȡ fds1��ȡ
		write(fds1[1], str, sizeof(str));
        //�ӽ���fds2����ȡ��Ϣ��������fds2д����Ϣ
		read(fds2[0], buf, BUF_SIZE);
		printf("Child proc output:%s\n", buf);
	}
	else {
		read(fds1[0], buf, BUF_SIZE);
		printf("Parent proc receive:%s\n", buf);
		write(fds2[1], str2, sizeof(str2));
	}

	return 0;
}
