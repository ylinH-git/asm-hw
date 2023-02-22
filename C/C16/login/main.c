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

char *decode(char *str, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(str[i] != 0)
		{
			str[i] = str[i] ^ '^';
		}
	}
	return str;
}

char * GetEncodeStr (char *encodeStr, char *password, char *login)
{
	crypto(password);
	crypto(login);
	strcpy(&encodeStr[32], password);
	strcpy(&encodeStr[64], login);
	return encodeStr;
}

unsigned int SDBMHash(char *str, int size)
{
	int i;
	unsigned int hash = 0;
	for(i = 0; i < size; i++)
	{
		hash = str[i] + (hash << 6) + (hash << 16) - hash;
	}
	return (hash & 0x7FFFFFFF);
}

unsigned int RSHash(char *str, int size)
{
	int i;
	unsigned int hash = 0, b = 378551, a = 63689;
	for(i = 0; i < size; i++)
	{
		hash = hash * a + str[i];
		a *= b;
	}
	return (hash & 0x7FFFFFFF);
}

int main(int argc, char *argv[])
{
	char encodeStr[96] = {(char)0xEB, (char)0x99, (char)0x9C,
		(char)0xE2, (char)0xED, (char)0x97, 
		(char)0xE7, (char)0xF8, (char)0xFD, 
		(char)0xFF};
	char successStr[32] = {0};
	int testCode = 0xfefefefe, i;
	char login[32],password[32];
	unsigned int SDBMHash1, RSHash1, inputSDBMHash, inputRSHash;
	strcpy(&encodeStr[32], "olmjkh");
	strcpy(&encodeStr[64], "?:370");
	SDBMHash1 = SDBMHash(encodeStr, 96);
	RSHash1 = RSHash(encodeStr, 96);

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
		GetEncodeStr(encodeStr, password, login);
		inputSDBMHash = SDBMHash(encodeStr, 96);
		inputRSHash = RSHash(encodeStr, 96);
		if(
			inputSDBMHash == SDBMHash1
			&& inputRSHash == RSHash1
			)
		{
			int j;
			decode(encodeStr, 96);
			for(j =0; j < 32; j++)
			{
				successStr[j] = encodeStr[j];
			}
			printf("%s\r\n", successStr);
			break;
		} else
		{
			if(times == 0)
			{
				printf("错误次数过多，程序退出\r\n");
				break;
			} 
			else 
			{
				printf("账号或密码错误\r\n");
			}
			continue;
		}
	}
	system("pause");
	return 0;
}