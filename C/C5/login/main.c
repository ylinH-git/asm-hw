#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRYTIMES 3
#define LOGINNAME "admin"
#define PASSWORD "123456"

int main(int argc, char *argv[])
{
	char login[20],password[20];
	int i;
	for(i = 0; i < TRYTIMES; i++)
	{
		int times = TRYTIMES - i - 1;
		printf("login:");
		scanf("%20s", login);
		if(strcmp(login, LOGINNAME) != 0)
		{
			if(times == 0)
			{
				printf("You have been locked!");
			} 
			else 
			{
				printf("The loginName is not found, please try again! %d more times will lock!\r\n", times);
			}
			continue;
		}
		printf("password:");
		scanf("%20s", password);
		if(strcmp(password, PASSWORD) != 0)
		{
			if(times == 0)
			{
				printf("You have been locked!");
			} 
			else 
			{
				printf("Wrong password, please try again!%d more times will lock!\r\n", times);
			}
			continue;
		} else
		{
			printf("µÇÂ½³É¹¦£¡\r\n");
			break;
		}
	}
	system("pause");
	return 0;
}