// strfunc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

size_t MyStrlen(const char* string)
{
	size_t size = 0;
	while (string[size] != '\0')
	{
		size++;
	}
	return size;
}

char* MyStrcpy(char* strDestination, const char* strSource)
{
	size_t i = 0;
	while (strSource[i] != '\0')
	{
		strDestination[i] = strSource[i];
		i++;
	}
	strDestination[i] = '\0';
	return strDestination;
}

char* MyStrcat(char* strDestination, const char* strSource)
{
	size_t firstEnd = MyStrlen(strDestination);
	size_t i = 0;
	while (strSource[i] != '\0')
	{
		strDestination[firstEnd + i] = strSource[i];
		i++;
	}
	strDestination[firstEnd + i] = '\0';
	return strDestination;

}

int main()
{
	char str[20] = "Hello World";
    cout << str << endl;
	cout << "strlen: " << MyStrlen(str) <<endl;
	cout << "strcpy(How much): " << MyStrcpy(str, "How much") << endl;
	cout << "strcat(How much + Worlds): " << MyStrcat(str, " Worlds") << endl;
}

