#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRYTIMES 3

char *crypto(char *str, char *str2)
{
	int i;
	for(i = 0; str2[i] != '\0'; i++)
	{
		if( i * i < 121) 
		{
			str[i] = str2[i] ^ 'k' + i * i;
		}
		else
		{
			str[i] = str2[i] ^ 'c' + 100;
		}
	}
	str[i + 1] = '\0';
	return str;
}

int main(int argc, char *argv[])
{
	char login[32],password[32];
	char enLogin[32] = "";
	char enPassword[32] = "";
	int i;

	for(i = 0; i < TRYTIMES; i++)
	{
		int times = TRYTIMES - i - 1;
		printf("login:");
		scanf("%31s", login);
		printf("password:");
		scanf("%31s", password);
		if(strcmp(crypto(password, password), crypto(enPassword, "123456")) != 0
			|| strcmp(crypto(login, login), crypto(enLogin, "admin")) != 0)
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