#ifndef DELETEFUNC_H
#define DELETEFUNC_H

/*
	rearrange the index table
*/
void ReArrangeIndexTable();

/*
	delete string by index and rearrange the index table
	params: int the index of string
	return: 0 means ok , 1 means delete failed
*/

int DeleteStringByIndex(int);

/*
	delete string in the free buf
	return: 0 means ok , 1 means delete failed
*/
int DeleteString();


#endif
