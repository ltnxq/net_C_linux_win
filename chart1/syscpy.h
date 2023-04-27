#pragma once
#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>
#define BUF_SIZE 3

int test_sysCopy() {
	int fd1, fd2;
	int len;
	char buf[BUF_SIZE];

	fd1 = open("Postman-win64-8.11.1-full.nupkg", O_RDONLY);
	fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC);

	//系统调用 read and  write
	while ((len = read(fd1,buf,sizeof(buf)))>0)
	{
		write(fd2, buf, len);
	}

	close(fd1);
	close(fd2);
	return 0;
}
