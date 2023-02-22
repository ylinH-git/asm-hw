#ifndef UNICODE_H
	#define UNICODE_H
	#ifdef UNICODE
		#define TCHAR wchar_t
		#define _tcscpy wcscpy
		#define _tcslen wcslen
		#define _tcscat wcscat
		#define _tprintf wprintf
		#define _tsetlocale _wsetlocale
		#define _tsystem _wsystem
		#define _L(x) L ## x
		#define _tmain wmain
	#else
		#define TCHAR char
		#define _tcscpy strcpy
		#define _tcslen strlen
		#define _tcscat strcat
		#define _tprintf printf
		#define _tsetlocale setlocale
		#define _tsystem system
		#define _L(x) x
		#define _tmain main
	#endif
#endif
