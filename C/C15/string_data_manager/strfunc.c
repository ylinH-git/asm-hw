#include "stdafx.h"

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