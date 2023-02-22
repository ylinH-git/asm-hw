#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRYTIMES 3

char *crypto(char *str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		str[i] = str[i] ^ '^';
	}
	return str;
}

char *decode(char *str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		str[i] = str[i] ^ '^';
	}
	return str;
}

int main(int argc, char *argv[])
{
	int testCode = 0xfefefefe, i;
	char login[32],password[32];

	for(i = 0; i < TRYTIMES; i++)
	{
		int times = TRYTIMES - i - 1;
		printf("login:");
		scanf("%31s", login);
		printf("password:");
		scanf("%31s", password);
		if(testCode != 0xfefefefe){
			exit(1);
		}
		if(
			strcmp(crypto(password), "olmjkh") != 0
			|| strcmp(crypto(login), "?:370") != 0
			)
		{
			if(times == 0)
			{
				printf("错误次数过多，程序退出\r\n");
				break;
			} 
			else 
			{
				printf("密码错误\r\n");
			}
			continue;
		} else
		{
			printf("登陆成功！\r\n");
			break;
		}
	}
	system("pause");
	return 0;
}