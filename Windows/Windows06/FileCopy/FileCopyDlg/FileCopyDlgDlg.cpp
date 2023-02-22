
// FileCopyDlgDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileCopyDlg.h"
#include "FileCopyDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CUS_STARTCOPY  WM_USER + 1


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


// CFileCopyDlgDlg 对话框



CFileCopyDlgDlg::CFileCopyDlgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECOPYDLG_DIALOG, pParent)
	, m_progressText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCopyDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PROGRESS_COPY, m_progress);
	DDX_Text(pDX, STATIC_TEXT, m_progressText);
}

BEGIN_MESSAGE_MAP(CFileCopyDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_START, &CFileCopyDlgDlg::OnBnClickedStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFileCopyDlgDlg 消息处理程序

BOOL CFileCopyDlgDlg::OnInitDialog()
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
	m_progressText = "等待开始";
	UpdateData(FALSE);
		
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileCopyDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileCopyDlgDlg::OnPaint()
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
HCURSOR CFileCopyDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

__declspec(dllimport) extern int g_progress;
__declspec(dllimport) extern char g_sourcePath[MAX_PATH];
__declspec(dllimport) extern char g_destPath[MAX_PATH];
#pragma comment(lib, "../Debug/ShareDll.lib")

void CFileCopyDlgDlg::OnBnClickedStart()
{
	CString sourcePath;
	GetDlgItemText(EDT_SOURCE, sourcePath);
	CString destPath;
	GetDlgItemText(EDT_DEST, destPath);

	memcpy_s(g_sourcePath, MAX_PATH, sourcePath.GetBuffer(), sourcePath.GetLength() + 1);
	memcpy_s(g_destPath, MAX_PATH, destPath.GetBuffer(), sourcePath.GetLength() + 1);
	m_timer = SetTimer(1, 10, NULL);
	m_progressText = "进行中";
	UpdateData(FALSE);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	::ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	::ZeroMemory(&pi, sizeof(pi));

#if 1
	BOOL bRet = CreateProcess(
		NULL,
		"\"..\\Debug\\FileCopyControl.exe\"",
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);
	m_ctrlHandle = pi.hProcess;
#endif // 0


	
	//Sleep(300);
	if (bRet)
	{
		HWND hwndCtrl = ::FindWindow(NULL, "文件复制控制器");
		::ShowWindow(hwndCtrl, SW_HIDE);

#if 0
		COPYDATASTRUCT cdSource;
		cdSource.dwData = 0;
		cdSource.lpData = sourcePath.GetBuffer();
		cdSource.cbData = sourcePath.GetLength() + 1;

		::SendMessage(hwndCtrl, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&cdSource);

		COPYDATASTRUCT cdDest;
		cdDest.dwData = 1;
		cdDest.lpData = destPath.GetBuffer();
		cdDest.cbData = destPath.GetLength() + 1;

		::SendMessage(hwndCtrl, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&cdDest);
		m_timer = SetTimer(1, 10, NULL);

		::SendMessage(hwndCtrl, CUS_STARTCOPY, (WPARAM)GetSafeHwnd(), NULL);
		TerminateProcess(pi.hProcess, 0);
#endif // 0

	}

	CloseHandle(pi.hThread);
}


void CFileCopyDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == m_timer)
	{
		if (g_progress < 100)
		{
			m_progress.SetPos((int)g_progress);
		}
		else
		{
			m_progress.SetPos((int)g_progress);
			if (m_ctrlHandle != NULL) {
				CloseHandle(m_ctrlHandle);
			}
			m_progressText = "复制完成";
			UpdateData(FALSE);
			KillTimer(m_timer);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
