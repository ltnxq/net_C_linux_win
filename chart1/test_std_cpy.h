#pragma once

#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>
#define BUF_SIZE 3

int test_std_cpyt()
{
	FILE* fp1;
	FILE* fp2;
	char buf[BUF_SIZE];

	//���ñ�׼���IO���������п���ֲ��
	fp1 = fopen("Postman-win64-8.11.1-full.nupkg","r");
	fp2 = fopen("cpy.txt", "w");

	while (fgets(buf, BUF_SIZE, fp1) != NULL) {
		fputs(buf, fp2);
	}

	fclose(fp1);
	fclose(fp2);

	return 0;

}
