#ifndef MyStrFunc_H
#define MyStrFunc_H


/*
	计算字符长度串
	params: const char * 需要计算长度的字符串
	return size_t 长度
*/
size_t MyStrlen( const char *);

/*
	复制字符串
	params: char * 目标字符数组，const char * 被复制字符串
	return char * 字符数组
*/
char *MyStrcpy( char *, const char *);

/*
	拼接字符串
	params: char * 目标字符数组，const char * 被拼接字符串
	return char * 字符数组
*/
char *MyStrcat( char *, const char * );

/*
	对比字符串
	params: const char *  , char * 
	return int 相等0 小于-1 大于1
*/
int MyStrcmp( const char *, const char * );

/*
	转为小写字母
	params: char *
	return char *
*/
char *MyStrlwr( char * );

/*
	转为大写字母
	params: char *
	return char *
*/
char *MyStrupr( char * );

/*
	清空流中残余字符
*/
int clean();

/*
	安全读取流中字符
	params: char * 字符数组, size_t 字符数组安全长度
	return char * 字符数组
*/
char * sgetc(char *, size_t);
#endif