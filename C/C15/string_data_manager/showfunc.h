#ifndef SHOWFUNC_H
#define SHOWFUNC_H

/*
	show the buffer array
*/
void ShowBuf();

/*
	clear the screen through printf 25 \r\n rows
*/
void ClearScreen();

/*
	show the controller menu
*/
void ShowMenu();

/*
	show the search menu
*/
void ShowSearchMenu();

/*
	show the delete menu
*/
void ShowDeleteMenu();

/*
	show the change menu
*/
void ShowChangeMenu();

/*
	show the search info
	params int index of string, int address, int length of string, char * string
*/
void ShowSearchInfo(int, int, int, char *);

/*
	show the search error
	params int error code
*/
void ShowSearchErr(int);

/*
	show the delete error
	params int error code
*/
void ShowDeleteErr(int);

/*
	show the change error
	params int error code 1 not found 2 not enough buff
*/
void ShowChangeErr(int);

/*
	show the count of characters ASCII(37 - 126)
	params int * ASCII array, int size
*/
void ShowCount(int *, int);

/*
	show the write error
	params int error code
*/
void ShowWriteErr(int);
#endif