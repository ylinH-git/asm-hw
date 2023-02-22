
// FileCopyControlDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileCopyControl.h"
#include "FileCopyControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define CUS_STARTCOPY  WM_USER + 1

__declspec(dllimport) extern int g_progress;
__declspec(dllimport) extern char g_sourcePath[MAX_PATH];
__declspec(dllimport) extern char g_destPath[MAX_PATH];
#pragma comment(lib, "../Debug/ShareDll.lib")

int GetHigh(__int64 nBigNum)
{
	return nBigNum >> 32;
}

int GetLow(__int64 nBigNum)
{
	return (int)nBigNum;
}

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


// CFileCopyControlDlg 对话框



CFileCopyControlDlg::CFileCopyControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECOPYCONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCopyControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileCopyControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_MESSAGE(CUS_STARTCOPY, &CFileCopyControlDlg::OnCusStartcopy)
END_MESSAGE_MAP()


// CFileCopyControlDlg 消息处理程序

BOOL CFileCopyControlDlg::OnInitDialog()
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
	CopyFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileCopyControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileCopyControlDlg::OnPaint()
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
HCURSOR CFileCopyControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CFileCopyControlDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pCopyDataStruct->dwData == 0)
	{
		m_sourcePath.Format("%s", (char*)pCopyDataStruct->lpData);
	}

	if (pCopyDataStruct->dwData == 1)
	{
		m_destPath.Format("%s", (char*)pCopyDataStruct->lpData);
	}

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

afx_msg LRESULT CFileCopyControlDlg::OnCusStartcopy(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CFileCopyControlDlg::CopyFile()
{
	__int64 nCopyTotal = 0;
	HANDLE hSourceFile = CreateFile(g_sourcePath,
		GENERIC_READ | GENERIC_WRITE, //可读可写
		FILE_SHARE_READ, //允许别的进程读取
		NULL,
		OPEN_EXISTING, //打开已经存在
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	HANDLE hDestFile = CreateFile(g_destPath,
		GENERIC_READ | GENERIC_WRITE, //可读可写
		FILE_SHARE_READ, //允许别的进程读取
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hSourceFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("源文件打开失败");
		ExitProcess(0);
	}

	if (hDestFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSourceFile);
		AfxMessageBox("目标文件打开失败, 可能存在同名文件");
		ExitProcess(0);
	}

	DWORD dwHigh = 0;
	DWORD dwSize = ::GetFileSize(hSourceFile, &dwHigh);
	__int64 nSize = ((__int64)dwHigh << 32) + dwSize;
	HANDLE hSourceMap = CreateFileMapping(hSourceFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hSourceMap == NULL)
	{
		AfxMessageBox("源文件映射对象创建失败");
		CloseHandle(hSourceFile);
		CloseHandle(hDestFile);
		ExitProcess(0);
	}

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	do {

		__int64 nRestSize = nSize - nCopyTotal;
		g_progress = (double)((((double)nCopyTotal) / nSize) * 100);
		if (g_progress >= 100)
		{
			break;
		}
		LPVOID pSourceData = NULL;
		int chunkSize = 0;
		if (nRestSize < si.dwAllocationGranularity)
		{
			chunkSize = (int)nRestSize;
		}
		else
		{
			chunkSize = si.dwAllocationGranularity;
		}
		pSourceData = MapViewOfFile(hSourceMap, FILE_MAP_ALL_ACCESS, GetHigh(nCopyTotal), GetLow(nCopyTotal), (int)chunkSize);
		auto err = GetLastError();
		if (pSourceData == NULL)
		{
			AfxMessageBox("源文件映射进内存失败");
			break;
		}
		DWORD nBeWritten = 0;
		OVERLAPPED ol = { NULL, NULL,{ 0xffffffff, 0xffffffff }, NULL };
		if (!WriteFile(hDestFile, pSourceData, chunkSize, &nBeWritten, &ol))
		{
			AfxMessageBox("文件写入失败");
			break;
		}
		//清理
		UnmapViewOfFile(pSourceData);
		nCopyTotal += chunkSize;
	} while (g_progress <= 100);

	CloseHandle(hSourceMap);
	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);
	ExitProcess(0);
}

