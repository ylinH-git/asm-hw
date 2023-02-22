#include <stdlib.h>
#include <locale.h>
#include "MySnakeControl.h"
#include "unicode.h"

int _tmain (int argc, TCHAR *argv[])
{
	_tsetlocale(LC_ALL, _L("chs"));
	StartGame();
	_tsystem(_L("pause"));
	return 0;
}