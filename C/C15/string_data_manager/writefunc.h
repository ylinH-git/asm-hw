#ifndef WRITEFUNC_H
#define WRITEFUNC_H

/*
	write input string to free buf and set the index table
	params: const char * input string 
	return: 0 means ok , 1 means string overflow, 2 means not enough buf
*/
int WriteStrToBuf (const char *);

#endif