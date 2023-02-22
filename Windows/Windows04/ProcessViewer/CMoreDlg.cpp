// CMoreDlg.cpp: 实现文件
//

#include "pch.h"
#include "ProcessViewer.h"
#include "CMoreDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	CMoreDlg* pMoreDlg = (CMoreDlg*)lParam;
	auto pWindowDlg = &pMoreDlg->m_windowDlg;
	DWORD processID;
	DWORD threadID;
	threadID = ::GetWindowThreadProcessId(hwnd, &processID);
	if (processID == pMoreDlg->GetProcessId()) 
	{
		char title[MAXBYTE] = {};
		GetWindowText(hwnd, title, sizeof(title));
		pWindowDlg->m_windowList.InsertItem(0, title);
		CString winHandleStr;
		winHandleStr.Format("%p", hwnd);
		pWindowDlg->m_windowList.SetItemText(0, 1, winHandleStr);
		CString processStr;
		processStr.Format("%08x", processID);
		pWindowDlg->m_windowList.SetItemText(0, 2, processStr);
		auto instanceHandle = ::GetWindowLong(hwnd, GWL_HINSTANCE);
		CString insHandleStr;
		insHandleStr.Format("%08x", instanceHandle);
		pWindowDlg->m_windowList.SetItemText(0, 3, insHandleStr);
		auto menuHwnd = ::GetMenu(hwnd);
		CString menuHandleStr;
		menuHandleStr.Format("%p", menuHwnd);
		pWindowDlg->m_windowList.SetItemText(0, 4, menuHandleStr);
		auto userData = ::GetWindowLong(hwnd, GWL_USERDATA);
		CString userDataStr;
		userDataStr.Format("%08x", userData);
		pWindowDlg->m_windowList.SetItemText(0, 5, userDataStr);
	}
	return TRUE;
}

// CMoreDlg 对话框

IMPLEMENT_DYNAMIC(CMoreDlg, CDialogEx)

CMoreDlg::CMoreDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MORE_DIALOG, pParent)
{

}

CMoreDlg::~CMoreDlg()
{
}

void CMoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, MORE_TAB, m_tab);
}

void CMoreDlg::SetProcessId(DWORD processId)
{
	m_processId = processId;
}

DWORD CMoreDlg::GetProcessId()
{
	return m_processId;
}


BEGIN_MESSAGE_MAP(CMoreDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, MORE_TAB, &CMoreDlg::OnSelchangeMoreTab)
END_MESSAGE_MAP()


// CMoreDlg 消息处理程序


BOOL CMoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int nIdx = 0;
	m_tab.InsertItem(nIdx++, "线程");
	m_tab.InsertItem(nIdx++, "模块");
	m_tab.InsertItem(nIdx++, "窗口");

	CRect rc;
	m_tab.GetClientRect(&rc);

	InitThreadDlg();
	InitModuleDlg();
	InitWindowDlg();

	rc.top += 25;
	rc.left += 5;
	rc.right -= 5;
	rc.bottom -= 5;

	m_threadDlg.MoveWindow(&rc);
	m_moduleDlg.MoveWindow(&rc);
	m_windowDlg.MoveWindow(&rc);

	m_threadDlg.ShowWindow(SW_SHOW);
	m_moduleDlg.ShowWindow(SW_HIDE);
	m_windowDlg.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMoreDlg::InitWindowDlg()
{
	m_windowDlg.Create(IDD_WINDOW_DIALOG, &m_tab);
	m_windowDlg.m_windowList.SetExtendedStyle(m_windowDlg.m_windowList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	m_windowDlg.m_windowList.InsertColumn(1, "窗口标题");
	m_windowDlg.m_windowList.InsertColumn(2, "窗口句柄");
	m_windowDlg.m_windowList.InsertColumn(3, "窗口进程");
	m_windowDlg.m_windowList.InsertColumn(4, "实例句柄");
	m_windowDlg.m_windowList.InsertColumn(5, "菜单句柄");
	m_windowDlg.m_windowList.InsertColumn(6, "用户数据");
	EnumWindows(EnumWindowsProc, (LPARAM)this);
	SetWindowListWidth();
}

void CMoreDlg::SetWindowListWidth()
{
	m_windowDlg.m_windowList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_windowDlg.m_windowList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_windowDlg.m_windowList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_windowDlg.m_windowList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_windowDlg.m_windowList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
	m_windowDlg.m_windowList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);
}

void CMoreDlg::InitModuleDlg()
{
	m_moduleDlg.Create(IDD_MODULE_DIALOG, &m_tab);
	m_moduleDlg.m_moduleList.SetExtendedStyle(m_moduleDlg.m_moduleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_moduleDlg.m_moduleList.InsertColumn(1, "MODULE NAME");
	m_moduleDlg.m_moduleList.InsertColumn(2, "Executable");
	m_moduleDlg.m_moduleList.InsertColumn(3, "Process ID");
	m_moduleDlg.m_moduleList.InsertColumn(4, "Ref count (g)");
	m_moduleDlg.m_moduleList.InsertColumn(5, "Ref count (p)");
	m_moduleDlg.m_moduleList.InsertColumn(6, "Base address");
	m_moduleDlg.m_moduleList.InsertColumn(7, "Base size ");
	SetModuleListWidth();
	ListProcessModules();
}

BOOL CMoreDlg::ListProcessModules()
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_processId);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (!Module32First(hModuleSnap, &me32))
	{
		CloseHandle(hModuleSnap);           // clean the snapshot object
		return(FALSE);
	}

	// Now walk the module list of the process,
	// and display information about each module
	int nRowIdx = 0;
	do
	{
		m_moduleDlg.m_moduleList.InsertItem(nRowIdx, me32.szModule);
		m_moduleDlg.m_moduleList.SetItemText(nRowIdx, 1, me32.szExePath);
		CString processId;
		processId.Format("%08x", me32.th32ProcessID);
		m_moduleDlg.m_moduleList.SetItemText(nRowIdx, 2, processId);
		CString glblcntUsage;
		glblcntUsage.Format("%04x", me32.GlblcntUsage);
		m_moduleDlg.m_moduleList.SetItemText(nRowIdx, 3, glblcntUsage);
		CString proccntUsage;
		proccntUsage.Format("%04x", me32.ProccntUsage);
		m_moduleDlg.m_moduleList.SetItemText(nRowIdx, 4, proccntUsage);
		CString modBaseAddr;
		modBaseAddr.Format("%p", me32.modBaseAddr);
		m_moduleDlg.m_moduleList.SetItemText(nRowIdx, 5, modBaseAddr);
		CString modBaseSize;
		modBaseSize.Format("%d", me32.modBaseSize);
		m_moduleDlg.m_moduleList.SetItemText(nRowIdx++, 6, modBaseSize);

	} while (Module32Next(hModuleSnap, &me32));
	SetModuleListWidth();
	CloseHandle(hModuleSnap);
	return(TRUE);
}

void CMoreDlg::SetModuleListWidth()
{
	m_moduleDlg.m_moduleList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleDlg.m_moduleList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleDlg.m_moduleList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleDlg.m_moduleList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleDlg.m_moduleList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleDlg.m_moduleList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleDlg.m_moduleList.SetColumnWidth(6, LVSCW_AUTOSIZE_USEHEADER);
}

void CMoreDlg::InitThreadDlg()
{
	m_threadDlg.Create(IDD_THREAD_DIALOG, &m_tab);
	m_threadDlg.m_threadList.SetExtendedStyle(m_threadDlg.m_threadList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_threadDlg.m_threadList.InsertColumn(1, "THREAD ID");
	m_threadDlg.m_threadList.InsertColumn(2, "Base priority");
	m_threadDlg.m_threadList.InsertColumn(3, "OwnerProcessID");
	SetThreadListWidth();
	ListProcessThreads();
}

BOOL CMoreDlg::ListProcessThreads()
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		CloseHandle(hThreadSnap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	int nRowIdx = 0;
	do
	{
		if (te32.th32OwnerProcessID == m_processId)
		{
			CString threadId;
			threadId.Format("%08x", te32.th32ThreadID);
			m_threadDlg.m_threadList.InsertItem(nRowIdx, threadId);
			CString basePri;
			basePri.Format("%d", te32.tpBasePri);
			m_threadDlg.m_threadList.SetItemText(nRowIdx, 1, basePri);
			CString ownerId;
			ownerId.Format("%08x", te32.th32OwnerProcessID);
			m_threadDlg.m_threadList.SetItemText(nRowIdx++, 2, ownerId);
		}
	} while (Thread32Next(hThreadSnap, &te32));
	SetThreadListWidth();

	CloseHandle(hThreadSnap);
	return(TRUE);
}

void CMoreDlg::SetThreadListWidth()
{
	m_threadDlg.m_threadList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_threadDlg.m_threadList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_threadDlg.m_threadList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}


void CMoreDlg::OnSelchangeMoreTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nIdx = m_tab.GetCurSel();
	m_threadDlg.ShowWindow(SW_HIDE);
	m_moduleDlg.ShowWindow(SW_HIDE);
	m_windowDlg.ShowWindow(SW_HIDE);

	switch (nIdx)
	{
	case 0:
		m_threadDlg.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_moduleDlg.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_windowDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}
