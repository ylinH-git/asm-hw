#ifndef CONTROLLER_H
#define CONTROLLER_H

/*
	cheak the length of string in the range(1-100),contains \0 
	params: char * 
	return: 0 means ok , 1 means overflow
*/
int CheakString (const char *);

/*
	cheak the number in the range(0 - 4095)
	return: 0 means ok , 1 means overflow
*/
int CheakNum (int);

/*
	find free buf for input string
	params: const char * input string 
	return: -1 means not enough buf , other number means address
*/
int FindFreeBuf (const char *);

/*
	excuted the menu command
	return 0 means exit
*/
int Command();
#endif