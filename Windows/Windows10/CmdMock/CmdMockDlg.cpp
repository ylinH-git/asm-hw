
// CmdMockDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CmdMock.h"
#include "CmdMockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DWORD WINAPI RunThreadProce(LPVOID lpParameter)
{
	CCmdMockDlg* dlg = (CCmdMockDlg*)lpParameter;

	while (true)
	{
		DWORD dwBytesAvail = 0;
		if (PeekNamedPipe(dlg->m_hParentRead, NULL, 0, NULL, &dwBytesAvail, NULL))
		{
			if (dwBytesAvail > 0)
			{
				CString lastText;
				HWND resultHwnd = GetDlgItem(dlg->GetSafeHwnd(), EDT_RESULT);
				CEdit rEdit;
				rEdit.Attach(resultHwnd);
				rEdit.GetWindowText(lastText);
				DWORD dwByteReaded = 0;
				CString strBuf;
				auto bRet = ReadFile(dlg->m_hParentRead,
					strBuf.GetBufferSetLength(MAXBYTE),
					MAXBYTE,
					&dwByteReaded,
					NULL);
				strBuf.ReleaseBuffer(dwByteReaded);
				rEdit.SetWindowText(lastText + strBuf);
				rEdit.Detach();
				dlg->m_resultEdit.LineScroll(dlg->m_resultEdit.GetLineCount());
			}
			else
			{
				Sleep(1);
			}
		}
		else
		{
			Sleep(1);
		}
	}
	return 0;
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


// CCmdMockDlg 对话框



CCmdMockDlg::CCmdMockDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMDMOCK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCmdMockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDT_RESULT, m_resultEdit);
}

BEGIN_MESSAGE_MAP(CCmdMockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_RUN, &CCmdMockDlg::OnBnClickedRun)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCmdMockDlg 消息处理程序

BOOL CCmdMockDlg::OnInitDialog()
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

	// 创建一个作业 用来关闭主进程时关闭子进程
	HANDLE ghJob = CreateJobObject(NULL, NULL); // GLOBAL
	if (ghJob == NULL)
	{
		::MessageBox(0, "Could not create job object", "TEST", MB_OK);
	}
	else
	{
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

		// Configure all child processes associated with the job to terminate when the
		jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
		if (0 == SetInformationJobObject(ghJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
		{
			::MessageBox(0, "Could not SetInformationJobObject", "TEST", MB_OK);
		}
	}


	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	if (!CreatePipe(&m_hChildRead, &m_hParentWrite, &sa, 0))
	{
		AfxMessageBox("失败");
	}

	if (!CreatePipe(&m_hParentRead, &m_hChildWrite, &sa, 0))
	{
		AfxMessageBox("失败");
	}

	STARTUPINFO si = {};
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = m_hChildRead;
	si.hStdOutput = m_hChildWrite;
	PROCESS_INFORMATION pi = {};
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(
		NULL,
		"\"cmd.exe\"",
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi
	))
	{
		AfxMessageBox("Failed");
	}

	if (ghJob)
	{
		if (0 == AssignProcessToJobObject(ghJob, pi.hProcess))
		{
			::MessageBox(0, "Could not AssignProcessToObject", "TEST", MB_OK);
		}
	}


	m_hThread = CreateThread(
		NULL,
		0,              //使用默认的栈大小
		RunThreadProce, //线程回调函数地址，新线程从这个函数开始执行代码
		this,           //自定参数，会传递给线程回调函数
		0,              //线程立即运行
		NULL            //线程id，不需要
	);
	if (m_hThread == NULL)
	{
		AfxMessageBox("线程创建失败");
	}
	CloseHandle(m_hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCmdMockDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCmdMockDlg::OnPaint()
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
HCURSOR CCmdMockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCmdMockDlg::OnBnClickedRun()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD dwBytesWrited = 0;
	CString strText;
	GetDlgItemText(EDT_COMMAND, strText);
	strText += "\r\n";
	WriteFile(m_hParentWrite,
		strText.GetBuffer(),
		strText.GetLength(),
		&dwBytesWrited,
		NULL);
}


void CCmdMockDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnTimer(nIDEvent);
}
