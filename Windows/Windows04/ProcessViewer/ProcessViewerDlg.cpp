
// ProcessViewerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ProcessViewer.h"
#include "ProcessViewerDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include "CMoreDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessViewerDlg 对话框



CProcessViewerDlg::CProcessViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESSVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_PROCESS, m_processList);
}

BEGIN_MESSAGE_MAP(CProcessViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, LIST_PROCESS, &CProcessViewerDlg::OnDblclkListProcess)
END_MESSAGE_MAP()


// CProcessViewerDlg 消息处理程序

BOOL CProcessViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_processList.SetExtendedStyle(m_processList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_AUTOSIZECOLUMNS);
	m_processList.InsertColumn(1, "Process Name");
	m_processList.InsertColumn(2, "Process ID");
	m_processList.InsertColumn(3, "Thread count");
	m_processList.InsertColumn(4, "Parent process ID");
	m_processList.InsertColumn(5, "Priority base");
	SetColumnWidthAuto();
	GetProcessList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProcessViewerDlg::SetColumnWidthAuto()
{
	m_processList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_processList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_processList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_processList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_processList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
}

BOOL CProcessViewerDlg::GetProcessList()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		AfxMessageBox(TEXT("Process32First"));
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	int nRowIdx = 0;
	do
	{
		m_processList.InsertItem(nRowIdx, pe32.szExeFile);
		CString processId;
		processId.Format("%08x", pe32.th32ProcessID);
		m_processList.SetItemText(nRowIdx, 1, processId);
		CString threads;
		threads.Format("%d", pe32.cntThreads);
		m_processList.SetItemText(nRowIdx, 2, threads);
		CString parentProcessId;
		parentProcessId.Format("%08x", pe32.th32ParentProcessID);
		m_processList.SetItemText(nRowIdx, 3, parentProcessId);
		CString classBase;
		classBase.Format("%d", pe32.pcPriClassBase);
		m_processList.SetItemText(nRowIdx++, 4, classBase);
	} while (Process32Next(hProcessSnap, &pe32));
	SetColumnWidthAuto();
	CloseHandle(hProcessSnap);
	return(TRUE);
}

void CProcessViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProcessViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProcessViewerDlg::OnDblclkListProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int idx = m_processList.GetSelectionMark();
	char processId[MAXBYTE] = {};
	m_processList.GetItemText(idx, 1, processId, sizeof(processId));
	int nProcessId = std::stoi(processId, nullptr, 16);
	CMoreDlg dlg;
	dlg.SetProcessId(nProcessId);
	dlg.DoModal();
}
