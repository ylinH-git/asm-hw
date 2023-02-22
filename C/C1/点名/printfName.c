#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TIMES 30

void printAll(char names[70][81], int i)
{
	int j;
	printf("����ѧ������:\n");
	for(j = 0; j < i; j++) {
		printf("%s, ", names[j]);
	}
}

void randomCall(char names[70][81], int i)
{
	int a, j;
	srand((unsigned)time(NULL));
	printf("���%d��ѧ������:\n", TIMES);
	for(j = 0; j < TIMES; j++) {
		a = rand() % i;
		printf("%s\n", names[a]);
	}
}

int has(int *called, int len, int num) {
	int c;
	for(c = 0; c < len; c++) {
		if(called[c] == num) {
			return 1;
		}
	}
	return 0;
}

void randomCallNoRepeat(char names[70][81], int i)
{
	int a, j, c;
	int *called = malloc(TIMES * sizeof(int));
	srand((unsigned)time(NULL));
	printf("������ظ�%d��ѧ������:\n", TIMES);
	for(j = 0; j < TIMES; j++) {
		called[j] = rand() % i;
		while(has(called, j, called[j])) {
			called[j] = rand() % i;
		}
		printf("%s\n", names[called[j]]);
	}
}

int main(int argc, char *argv[]) 
{
	char students[70][81];
	FILE *fp;
	char *student;
	char *fileName = argv[1];
	char *command = argv[2];
	int i;
	int end = 0;
	if (argc == 0) {
		printf("�������ļ���");
		return 1;
	} 
	fp = fopen( fileName, "r" );
	if (fp == NULL) {
		printf("���ļ�ʧ��");
		return 1;
	}
	i = 0;
	while(!feof(fp)) {
		fscanf(fp,"%s", students[i]);
		i++;
		if(i > 69) {
			printf("�ѳ���70��ѧ��");
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);

	if(strcmp(command, "all") == 0) {
		printAll(students, i);
	}

	if(strcmp(command, "call") == 0) {
		randomCall(students, i);
	}

	if(strcmp(command, "nrcall") == 0) {
		randomCallNoRepeat(students, i);
	}
	return 0;
}