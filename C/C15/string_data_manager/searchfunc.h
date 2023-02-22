#ifndef SEARCHFUNC_H
#define SEARCHFUNC_H

/*
	find string in the index mode
	return: 0 means ok , 1 means error
*/
int FindStringIndexMode();

/*
	find string in the string match mode
	return: 0 means ok , 1 means error
*/
int FindStringStrMode();

/*
	find string in the address match mode
	return: 0 means ok , 1 means error
*/
int FindStringAddrMode();

/*
	find string in the buff
	return: 0 means ok , 1 means error
*/
int FindString();
#endif