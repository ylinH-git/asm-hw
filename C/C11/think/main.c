#include <stdio.h>
#include <stdlib.h>


int clean()
{
	while(getchar() != '\n'){}
	return 0;
}

char * sgetc(char *string, size_t size){
	size_t i = 0;
	char c;
	while(i < size - 1 && (c = getchar()) != '\n')
	{
		string[i++] = c;
	}
	string[i] = '\0';
	if(c != '\n')
	{
		clean();
	}
	return string;
}

int main(int argc, char* argv[])
{
	char input[256] = {0}; 
	int word = 0, i;

	printf("请输入255长度内的字符串:\r\n");
	sgetc(input, 256);

	for(i = 0; i < 256; i++)
	{
		if(input [i + 1] == '\0')
		{
			break;
		}
		if(input[i] == ' ' && input[i + 1] != ' ')
		{
			word++;
		}
	}
	if(i > 0)
	{
		word++;
	}
	printf("合计有%d个单词\r\n", word);
	system("pause");
	return 0;
}