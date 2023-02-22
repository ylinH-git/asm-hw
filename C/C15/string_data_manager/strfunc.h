#ifndef MyStrFunc_H
#define MyStrFunc_H


/*
	�����ַ����ȴ�
	params: const char * ��Ҫ���㳤�ȵ��ַ���
	return size_t ����
*/
size_t MyStrlen( const char *);

/*
	�����ַ���
	params: char * Ŀ���ַ����飬const char * �������ַ���
	return char * �ַ�����
*/
char *MyStrcpy( char *, const char *);

/*
	ƴ���ַ���
	params: char * Ŀ���ַ����飬const char * ��ƴ���ַ���
	return char * �ַ�����
*/
char *MyStrcat( char *, const char * );

/*
	�Ա��ַ���
	params: const char *  , char * 
	return int ���0 С��-1 ����1
*/
int MyStrcmp( const char *, const char * );

/*
	תΪСд��ĸ
	params: char *
	return char *
*/
char *MyStrlwr( char * );

/*
	תΪ��д��ĸ
	params: char *
	return char *
*/
char *MyStrupr( char * );

/*
	������в����ַ�
*/
int clean();

/*
	��ȫ��ȡ�����ַ�
	params: char * �ַ�����, size_t �ַ����鰲ȫ����
	return char * �ַ�����
*/
char * sgetc(char *, size_t);
#endif