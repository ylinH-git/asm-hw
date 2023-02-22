// start.cpp : 定义应用程序的入口点。
//

typedef wchar_t WCHAR;
#define _Null_terminated_ _SA_annotes3(SAL_name, #Name, "", "2")
#define APIENTRY  __stdcall
#define _Group_impl_(annos)
#define _SA_annotes3(n,pp1,pp2,pp3)
#define _Group_(annos)                 _Group_impl_(annos _SAL_nop_impl_)
#define _SAL2_Source_(Name, args, annotes) _SA_annotes3(SAL_name, #Name, "", "2") _Group_(annotes _SAL_nop_impl_)
#define _In_                            _SAL2_Source_(_In_, (), _Pre1_impl_(__notnull_impl_notref) _Pre_valid_impl_ _Deref_pre1_impl_(__readaccess_impl_notref))
#define _In_opt_                        _SAL2_Source_(_In_opt_, (), _Pre1_impl_(__maybenull_impl_notref) _Pre_valid_impl_ _Deref_pre_readonly_)
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
DECLARE_HANDLE(HINSTANCE);

typedef _Null_terminated_ WCHAR* NWPSTR, * LPWSTR, * PWSTR;

#define HINSTANCE void*
#ifndef CONST
#define CONST               const
#endif
typedef char CHAR;
typedef _Null_terminated_ CONST CHAR* LPCSTR, * PCSTR;
typedef _Null_terminated_ CONST WCHAR* LPCWSTR, * PCWSTR;
typedef unsigned int        UINT;
DECLARE_HANDLE(HWND);


#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINUSERAPI DECLSPEC_IMPORT

WINUSERAPI
int
__stdcall MessageBoxA(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType);
WINUSERAPI
int
__stdcall
MessageBoxW(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType);
#ifdef UNICODE
#define TEXT(quote) L##quote 
#define MessageBox  MessageBoxW
#else
#define MessageBox  MessageBoxA
#define TEXT(quote) quote 
#endif // !UNICODE



#ifndef NOMB

/*
 * MessageBox() Flags
 */
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#if(WINVER >= 0x0500)
#define MB_CANCELTRYCONTINUE        0x00000006L
#endif /* WINVER >= 0x0500 */


#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L

#if(WINVER >= 0x0400)
#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND
#endif /* WINVER >= 0x0400 */

#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND

#define MB_DEFBUTTON1               0x00000000L
#define MB_DEFBUTTON2               0x00000100L
#define MB_DEFBUTTON3               0x00000200L
#if(WINVER >= 0x0400)
#define MB_DEFBUTTON4               0x00000300L
#endif /* WINVER >= 0x0400 */

#define MB_APPLMODAL                0x00000000L
#define MB_SYSTEMMODAL              0x00001000L
#define MB_TASKMODAL                0x00002000L
#if(WINVER >= 0x0400)
#define MB_HELP                     0x00004000L // Help Button
#endif /* WINVER >= 0x0400 */

#define MB_NOFOCUS                  0x00008000L
#define MB_SETFOREGROUND            0x00010000L
#define MB_DEFAULT_DESKTOP_ONLY     0x00020000L

#if(WINVER >= 0x0400)
#define MB_TOPMOST                  0x00040000L
#define MB_RIGHT                    0x00080000L
#define MB_RTLREADING               0x00100000L

#endif /* WINVER >= 0x0400 */

#ifdef _WIN32_WINNT
#if (_WIN32_WINNT >= 0x0400)
#define MB_SERVICE_NOTIFICATION          0x00200000L
#else
#define MB_SERVICE_NOTIFICATION          0x00040000L
#endif
#define MB_SERVICE_NOTIFICATION_NT3X     0x00040000L
#endif

#define MB_TYPEMASK                 0x0000000FL
#define MB_ICONMASK                 0x000000F0L
#define MB_DEFMASK                  0x00000F00L
#define MB_MODEMASK                 0x00003000L
#define MB_MISCMASK                 0x0000C000L

#endif /* !NOMB */
#define NULL 0
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MessageBox(NULL, TEXT("hello world"), TEXT("tip"), MB_RETRYCANCEL);
    return 0;
}
