
// GameHelperDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GameHelper.h"
#include "GameHelperDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(BTN_UNBEATABLE, &CGameHelperDlg::OnBnClickedUnbeatable)
	ON_BN_CLICKED(BTN_AVOID, &CGameHelperDlg::OnBnClickedAvoid)
	ON_BN_CLICKED(BTN_SPEED, &CGameHelperDlg::OnBnClickedSpeed)
	ON_BN_CLICKED(BTN_BULLET, &CGameHelperDlg::OnBnClickedBullet)
	ON_WM_TIMER()
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

void CGameHelperDlg::GetBulletsNum()
{
	SIZE_T nBytesOfWrited = 0;
	BOOL bRet = ::ReadProcessMemory(
		m_hProc,
		(LPVOID)0x00406DA8,
		&m_bulletsNum,
		sizeof(m_bulletsNum),
		&nBytesOfWrited
	);
	SetDlgItemInt(EDT_BULLET, m_bulletsNum);
}
void CGameHelperDlg::GetHProc()
{
	m_hwndDst = ::FindWindow(NULL, "摿孭");
	DWORD dwPid = 0;
	::GetWindowThreadProcessId(m_hwndDst, &dwPid);
	m_hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

	BYTE speed = 0;
	SIZE_T nBytesOfWrited = 0;
	BOOL bRet = ::ReadProcessMemory(
		m_hProc,
		(LPVOID)0x004020F5,
		&speed,
		sizeof(speed),
		&nBytesOfWrited
	);
	SetDlgItemInt(EDT_SPEED, speed);
	GetBulletsNum();
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


void CGameHelperDlg::OnBnClickedUnbeatable()
{

	LPVOID baseAddress = (LPVOID) 0x00403616;
	BYTE btCode = 0xeb;
	SIZE_T changeSize = sizeof(btCode);
	SIZE_T nBytesOfWrited = 0;

	ChangeMemory(baseAddress, changeSize, &btCode, nBytesOfWrited);

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


void CGameHelperDlg::OnBnClickedAvoid()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_avoidTimer == NULL)
	{
		m_avoidTimer = SetTimer(1, 1, NULL);
	}
	else
	{
		KillTimer(m_avoidTimer);
		m_avoidTimer = NULL;
		delete[] m_bullets;
	}

}

void CGameHelperDlg::GetPlanePos(CPoint& plane)
{
	DWORD planeX = 0;
	DWORD planeY = 0;
	SIZE_T nBytesOfWrited = 0;
	BOOL bRet = ::ReadProcessMemory(
		m_hProc,
		(LPVOID)0x00406D6C,
		&planeX,
		sizeof(planeX),
		&nBytesOfWrited
	);
	bRet = ::ReadProcessMemory(
		m_hProc,
		(LPVOID)0x00406D70,
		&planeY,
		sizeof(planeY),
		&nBytesOfWrited
	);
	plane.SetPoint(planeX, planeY);
}


void CGameHelperDlg::OnBnClickedSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	LPVOID baseAddress = (LPVOID)0x004020F5;
	BYTE btCode = GetDlgItemInt(EDT_SPEED);
	SIZE_T changeSize = sizeof(btCode);
	SIZE_T nBytesOfWrited = 0;

	ChangeMemory(baseAddress, changeSize, &btCode, nBytesOfWrited);
}


void CGameHelperDlg::OnBnClickedBullet()
{
	// TODO: 在此添加控件通知处理程序代码
	LPVOID baseAddress = (LPVOID)0x00406DA8;
	DWORD btCode = GetDlgItemInt(EDT_BULLET);
	SIZE_T changeSize = sizeof(btCode);
	SIZE_T nBytesOfWrited = 0;

	m_bulletsNum = btCode;
	ChangeMemory(baseAddress, changeSize, &btCode, nBytesOfWrited);
}

BOOL CGameHelperDlg::NeedMove(const CPoint& plane)
{
	for (int i = 0; i < m_bulletsNum; i++)
	{
		int bulletX = (*((int*)&m_bullets[i * 0xf + 0]) >> 6) - 4;
		int bulletY = (*((int*)&m_bullets[i * 0xf + 4]) >> 6) - 4;
		if (abs(bulletX - plane.x) < 32 || abs(bulletY - plane.y) < 32)
		{
			return TRUE;
		}
	}
	return FALSE;

}

BOOL CGameHelperDlg::NoBullet(int x, int y)
{
	for (int i = 0; i < m_bulletsNum; i++)
	{
		int bulletX = (*((int*)&m_bullets[i * 0xf + 0]) >> 6) - 4;
		int bulletY = (*((int*)&m_bullets[i * 0xf + 4]) >> 6) - 4;
		if (abs(bulletX - x) < 10 || abs(bulletY - y) < 10)
		{
			return FALSE;
		}
	}
	return TRUE;

}

BOOL CGameHelperDlg::isDead()
{
	DWORD isDeadFlag = 0;
	SIZE_T nBytesOfWrited = 0;
	BOOL bRet = ::ReadProcessMemory(
		m_hProc,
		(LPVOID)0x00406D80,
		&isDeadFlag,
		sizeof(isDeadFlag),
		&nBytesOfWrited
	);
	return isDeadFlag != 0 ;
}
void CGameHelperDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetBulletsNum();
	if (m_bullets != nullptr)
	{
		delete[] m_bullets;
		m_bullets = nullptr;
	}  
	m_bullets = new unsigned char[0xf * m_bulletsNum];
	if (nIDEvent == m_avoidTimer && !isDead())
	{
		CPoint plane;
		GetPlanePos(plane);
		GetBullets();

		if (NeedMove(plane) && m_hwndDst != nullptr)
		{
			CWnd wnd;
			auto bRet = wnd.Attach(m_hwndDst);
			if (bRet)
			{
				CRect rc;
				wnd.GetClientRect(&rc);
				for (int h = 0; h < rc.Height(); h++)
				{
					for (int w = 0; w < rc.Width() ; w++)
					{
						if (NoBullet(w, h))
						{
							LPVOID baseXAddress = (LPVOID)0x00406D6C;
							LPVOID baseYAddress = (LPVOID)0x00406D70;
							SIZE_T nBytesOfWrited = 0;
							if (!isDead())
							{
								ChangeMemory(baseXAddress, sizeof(w), &w, nBytesOfWrited);
								ChangeMemory(baseYAddress, sizeof(h), &h, nBytesOfWrited);
							}
							break;
						}
					}
				}
			}
			wnd.Detach();
		}
		if (m_bullets != nullptr)
		{
			delete[] m_bullets;
			m_bullets = nullptr;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CGameHelperDlg::GetBullets()
{
	SIZE_T nBytesOfWrited = 0;
	BOOL bRet = ::ReadProcessMemory(
		m_hProc,
		(LPVOID)0x00406E10,
		m_bullets,
		0xf * m_bulletsNum,
		&nBytesOfWrited
	);
}
