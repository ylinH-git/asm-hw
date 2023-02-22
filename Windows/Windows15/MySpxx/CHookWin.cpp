// CHookWin.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CHookWin.h"
#include "afxdialogex.h"


// CHookWin 对话框

IMPLEMENT_DYNAMIC(CHookWin, CDialogEx)

CHookWin::CHookWin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_HOOK, pParent)
	, m_result(_T(""))
{

}

CHookWin::~CHookWin()
{
}

void CHookWin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_HOOKRESULT, m_result);
}


BEGIN_MESSAGE_MAP(CHookWin, CDialogEx)
	ON_BN_CLICKED(BTN_START, &CHookWin::OnBnClickedStart)
	ON_BN_CLICKED(BTN_CLOSE, &CHookWin::OnBnClickedClose)
END_MESSAGE_MAP()


// CHookWin 消息处理程序
#pragma comment(lib, "./Debug/hookDll.lib")
__declspec(dllimport) void InstallHook();
__declspec(dllimport)void UnInstallHook();
__declspec(dllimport) extern DWORD g_threadId;
__declspec(dllimport) extern HWND g_winHwnd;
__declspec(dllimport) extern HWND g_curHwnd;

void CHookWin::OnBnClickedStart()
{
	g_threadId = m_threadId;
	g_winHwnd = GetSafeHwnd();
	g_curHwnd = m_curHwnd;
	InstallHook();
}


void CHookWin::OnBnClickedClose()
{
	UnInstallHook();
	g_threadId = NULL;
	g_winHwnd = NULL;
	g_curHwnd = NULL;
}
