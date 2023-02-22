
// GameHelperDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GameHelper.h"
#include "GameHelperDlg.h"
#include "afxdialogex.h"
#include<psapi.h>

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


// CGameHelperDlg 对话框



CGameHelperDlg::CGameHelperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAMEHELPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameHelperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_GETHPROC, &CGameHelperDlg::OnBnClickedGethproc)
	ON_WM_TIMER()
	ON_BN_CLICKED(BTN_NEXT, &CGameHelperDlg::OnBnClickedNext)
	ON_BN_CLICKED(BTN_SUN, &CGameHelperDlg::OnBnClickedSun)
	ON_BN_CLICKED(BTN_GREEN, &CGameHelperDlg::OnBnClickedGreen)
END_MESSAGE_MAP()


// CGameHelperDlg 消息处理程序

BOOL CGameHelperDlg::OnInitDialog()
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
	GetHProc();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGameHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGameHelperDlg::OnPaint()
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

void CGameHelperDlg::GetHProc()
{
	m_hwndDst = ::FindWindow(NULL, "植物大战僵尸中文版");
	m_hGamebase = GetModuleHandle("PlantsVsZombies0.exe");
	DWORD dwPid = 0;
	::GetWindowThreadProcessId(m_hwndDst, &dwPid);
	m_hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	// 获取每一个模块加载基址
	HMODULE hModule[100] = { 0 };
	DWORD dwRet = 0;
	int num = 0;
	int bRet = EnumProcessModulesEx(m_hProc, (HMODULE*)(hModule), sizeof(hModule), &dwRet, NULL);
	if (bRet != 0) {
		m_hGamebase = hModule[0];
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGameHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameHelperDlg::OnBnClickedGethproc()
{
	// TODO: 在此添加控件通知处理程序代码
	CloseHandle(m_hProc);
	m_hProc = nullptr;
	m_hwndDst = nullptr;
	GetHProc();
}

void CGameHelperDlg::ChangeMemory(const LPVOID& baseAddress, const SIZE_T& changeSize, LPCVOID btCode, SIZE_T& nBytesOfWrited)
{
	DWORD dwOldProc = 0;
	BOOL bRet = ::VirtualProtectEx(m_hProc, baseAddress, changeSize, PAGE_READWRITE, &dwOldProc);

	bRet = ::WriteProcessMemory(m_hProc,
		baseAddress,  //写入的地址
		btCode, changeSize, //写入的数据 
		&nBytesOfWrited);//传出参数，成功写入多少个字节数

						 //还原
	::VirtualProtectEx(m_hProc, baseAddress, changeSize, dwOldProc, &dwOldProc);
}







void CGameHelperDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
}



void CGameHelperDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGameHelperDlg::OnBnClickedSun()
{
	long sunBase = (long)m_hGamebase + 0x3158f;
	BYTE btCode = 0xEB;
	SIZE_T changeSize = sizeof(btCode);
	SIZE_T nBytesOfWrited = 0;
	ChangeMemory((LPVOID)sunBase, changeSize, &btCode, nBytesOfWrited);

}


void CGameHelperDlg::OnBnClickedGreen()
{
	//获取当前进程中的LoadLibrary的地址（同一台电脑同一次启动时核心dll的地址都是一样的）
	HMODULE kernelHd = GetModuleHandle("kernel32");
	FARPROC pfnLoadLibrary = NULL;
	if (kernelHd)
	{
		pfnLoadLibrary = GetProcAddress(kernelHd, "LoadLibraryA");
	}
	// 写入dll地址
	char szDllPath[] = { R"(D:\51asm\homework\projects\plantsvszombies\GameHelpDll\Debug\GameHelpDll.dll)" };
	// 为三方程序申请一块内存并讲地址写入此内存内
	LPVOID pAddr = VirtualAllocEx(m_hProc, NULL, sizeof(szDllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	DWORD dwBytesWrited = 0;
	if (pAddr)
	{
		WriteProcessMemory(m_hProc, pAddr, szDllPath, sizeof(szDllPath), &dwBytesWrited);
	}

	// 创建远程线程，加载dll
	if (pfnLoadLibrary) {
		HANDLE hThread = CreateRemoteThread(m_hProc, NULL, 0,
			(LPTHREAD_START_ROUTINE)pfnLoadLibrary, //远程线程执行LoadLibrary
			pAddr, //dll路径在目标进程中的地址
			0, NULL);
		if (hThread)
		{
			WaitForSingleObject(hThread, INFINITE);
		}
	}

	if (pAddr)
	{
		VirtualFreeEx(m_hProc, pAddr, 0, MEM_RELEASE);
	}
}
