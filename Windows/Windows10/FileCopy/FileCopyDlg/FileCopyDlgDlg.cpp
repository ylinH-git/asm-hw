
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCopyDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PROGRESS_COPY, m_progress);
	DDX_Control(pDX, STATIC_TEXT, m_progressText);
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
	m_progressText.SetWindowText("等待复制");
		
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

DWORD WINAPI CopyThreadProce(LPVOID lpParameter)
{
	CFileCopyDlgDlg* dlg= (CFileCopyDlgDlg*)lpParameter;
	char sourcePath[MAXBYTE] = {};
	GetDlgItemText(dlg->GetSafeHwnd(),EDT_SOURCE, sourcePath, MAXBYTE);
	char destPath[MAXBYTE] = {};
	GetDlgItemText(dlg->GetSafeHwnd(),EDT_DEST, destPath, MAXBYTE);
	//打开文件
	HANDLE hSrcFile = CreateFile(sourcePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	LONGLONG nFileSize;
	GetFileSizeEx(hSrcFile, (PLARGE_INTEGER)&nFileSize);

	HANDLE hDstFile = CreateFile(destPath,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hDstFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}


	//拷贝文件
	LONGLONG nBytesAllReaede = 0;
	while (nBytesAllReaede < nFileSize)
	{
		DWORD dwBytesReaded = 0;
		DWORD dwBytesWirted = 0;
		BYTE buf[0x10000] = { 0 };
		ReadFile(hSrcFile,
			buf,
			sizeof(buf),
			&dwBytesReaded,
			NULL);
		WriteFile(hDstFile,
			buf,
			dwBytesReaded,
			&dwBytesWirted,
			NULL);

		nBytesAllReaede += dwBytesReaded;

		//发送进度
		double dbProgress = ((double)nBytesAllReaede / (double)nFileSize) * 100;
		if (dbProgress < 100)
		{
			dlg->m_progress.SetPos((int)dbProgress);
		}
		else
		{
			dlg->m_progress.SetPos((int)dbProgress);
			dlg->m_progressText.SetWindowText("复制完成");
		}
	}
	CloseHandle(hSrcFile);
	CloseHandle(hDstFile);
	dlg->m_hThread = NULL;
	return 0;
}

void CFileCopyDlgDlg::OnBnClickedStart()
{
	if (m_hThread == NULL) 
	{
		m_progressText.SetWindowText("等待复制");
		m_hThread = CreateThread(
			NULL,
			0,              //使用默认的栈大小
			CopyThreadProce, //线程回调函数地址，新线程从这个函数开始执行代码
			this,           //自定参数，会传递给线程回调函数
			0,              //线程立即运行
			NULL            //线程id，不需要
		);
		if (m_hThread == NULL)
		{
			AfxMessageBox("线程创建失败");
			return;
		}
		CloseHandle(m_hThread);
	}
	else
	{
		AfxMessageBox("正在复制中");
	}
	return;
}
