#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char*argv[])
{

	int i;
	int month[12] = {0};
	char months[12][12] = {
		"january",
		"february",
		"march",
		"april",
		"may",
		"june",
		"july",
		"august",
		"september",
		"october",
		"november",
		"december"
	};

	printf("�����·ݵ�Ӣ�ģ�ȫƴ��");
	scanf("%11s", month);

	for(i = 0; i < sizeof(months) / sizeof(months[0]); i++)
	{
		if(strcmp(months, _strlwr(month)) == 0)
		{
			break;
		}
	}

	if(i < 12)
	{
		printf("��ǰ�·�Ϊ%d��\r\n", i + 1);	
	}
	else
	{
		printf("δ��ѯ����Ӧ����\r\n");	
	}

	system("pause");
	return 0;
}
