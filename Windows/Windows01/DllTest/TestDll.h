#pragma once
#ifdef DLLTEST_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif
DLLAPI void TestDllPrint();