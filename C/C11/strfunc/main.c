#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t MyStrlen( const char *string )
{
	size_t size = 0;
	while(string[size] != '\0')
	{
		size++;
	}
	return size;
}

char *MyStrcpy( char *strDestination, const char *strSource )
{
	size_t i = 0;
	while(strSource[i] != '\0')
	{
		strDestination[i] = strSource[i];
		i++;
	}
	strDestination[i] = '\0';
	return strDestination;
}

char *MyStrcat( char *strDestination, const char *strSource )
{
	size_t firstEnd = MyStrlen(strDestination);
	size_t i = 0;
	while(strSource[i] != '\0')
	{
		strDestination[firstEnd + i] = strSource[i];
		i++;
	}
	strDestination[firstEnd + i] = '\0';
	return strDestination;
	
}

int MyStrcmp( const char *string1, const char *string2 )
{
	int ret;
	int i = 0;
	while(!(ret = (int)string1[i] - (int)string2[i]) && string2[i] != '\0')
	{
		i++;
	}
	if(ret < 0)
	{
		return -1;
	}
	else if(ret > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char *MyStrlwr( char *string )
{
	size_t i = 0;
	while(string[i] != '\0')
	{
		if(string[i] <= 90 && string[i] >= 65)
		{
			string[i] = string[i] + 32;
		}
		i++;
	}
	return string;
}

char *MyStrupr( char *string )
{
	size_t i = 0;
	while(string[i] != '\0')
	{
		if(string[i] <= 122 && string[i] >= 97)
		{
			string[i] = string[i] - 32;
		}
		i++;
	}
	return string;
}

int main (int argc, char * argv[])
{

	char cpyStr[10];
	char catStr[12] = "hello ";
	char comStr1[] = "hello";
	char comStr2[] = "hello ";
	char loStr[] = "HELLO";
	char upStr[] = "hello";
	
	// strlen();
	printf("\"hello\" strlen: %d\r\n", MyStrlen("hello"));

	// strcpy();
	printf("copy \"123456789\" to  cpyStr[10]: %s\r\n", MyStrcpy(cpyStr, "123456789"));

	// strcat();
	printf("concat \"world\" to \"hello \": %s\r\n", MyStrcat(catStr, "world"));

	// strcmp();
	printf("compare \"hello\" to \"hello \": %d\r\n", MyStrcmp(comStr1, comStr2));
	printf("compare \"hello\" to \"hello\": %d\r\n", MyStrcmp("hello", "hello"));

	// strlwr();
	printf("lowercase \"HELLO\" to \"hello\": %s\r\n", MyStrlwr(loStr));

	// strupr();
	printf("uppercase \"hello\" to \"HELLO\": %s\r\n", MyStrupr(upStr));
	system("pause");
	return 0;
}