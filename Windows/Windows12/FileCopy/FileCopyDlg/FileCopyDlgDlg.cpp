
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
int GetHigh(LONGLONG nBigNum)
{
	return nBigNum >> 32;
}

int GetLow(LONGLONG nBigNum)
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
CRITICAL_SECTION g_csForVct;
LONGLONG g_nBytesAllReaede = 0;
HANDLE g_hSrcFile = INVALID_HANDLE_VALUE;
HANDLE g_hDstFile = INVALID_HANDLE_VALUE;
LONGLONG g_nFileSize = 0;
SYSTEM_INFO g_si;
DWORD WINAPI CopyThreadProce(LPVOID lpParameter)
{
	if (g_hSrcFile == INVALID_HANDLE_VALUE || g_hDstFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	HANDLE hSourceMap = CreateFileMapping(g_hSrcFile, NULL, PAGE_READONLY, 0, 0, NULL);
	HANDLE hDestMap = CreateFileMapping(g_hDstFile, NULL, PAGE_READWRITE, GetHigh(g_nFileSize), GetLow(g_nFileSize), NULL);
	if (hSourceMap == NULL)
	{
		AfxMessageBox("源文件映射对象创建失败");
		ClearFileHandle();
		return 0;
	}

	if (hDestMap == NULL)
	{
		AfxMessageBox("目标文件映射对象创建失败");
		CloseHandle(hSourceMap);
		ClearFileHandle();
		return 0;
	}
	tagThreadsParams* pParams= (tagThreadsParams*)lpParameter;
	CFileCopyDlgDlg* dlg = pParams->dlg;
	int nIdx = pParams->idx;
	//拷贝文件
	LONGLONG nBytesBatch = pParams->offset;
	LONGLONG nBytesAllRead = 0;
	while (nBytesBatch < pParams->endPos)
	{
		LONGLONG nRestSize = pParams->endPos - nBytesBatch;
		char* pSourceData = NULL;
		char* pDestData = NULL;
		int chunkSize = 0;
		if (nRestSize < g_si.dwAllocationGranularity)
		{
			chunkSize = (int)nRestSize;
		}
		else
		{
			chunkSize = g_si.dwAllocationGranularity;
		}
		pSourceData = (char*)MapViewOfFile(hSourceMap, FILE_MAP_READ | FILE_MAP_COPY, GetHigh(nBytesBatch), GetLow(nBytesBatch), (int)chunkSize);
		
		if (pSourceData == NULL)
		{
			AfxMessageBox("源文件映射进内存失败");
			break;
		}

		pDestData = (char*)MapViewOfFile(hDestMap, FILE_MAP_ALL_ACCESS, GetHigh(nBytesBatch), GetLow(nBytesBatch), (int)chunkSize);
		if (pDestData == NULL)
		{
			UnmapViewOfFile(pSourceData);
			AfxMessageBox("源文件映射进内存失败");
			break;
		}
		DWORD nBeWritten = 0;
		for (int i = 0; i < chunkSize; i++)
		{
			pDestData[i] = pSourceData[i];
		}
		//清理
		UnmapViewOfFile(pSourceData);
		UnmapViewOfFile(pDestData);
		nBytesBatch += chunkSize;
		nBytesAllRead += chunkSize;
		dlg->m_hThreadsBytesReaded[nIdx] = nBytesAllRead;
		EnterCriticalSection(&g_csForVct);
		g_nBytesAllReaede = 0;
		for (int i = 0; i < dlg->m_hThreadsNum; i++)
		{
			g_nBytesAllReaede += dlg->m_hThreadsBytesReaded[i];
		}
		//发送进度
		double dbProgress = ((double)g_nBytesAllReaede / (double)g_nFileSize) * 100;
		if (dbProgress < 100)
		{
			dlg->m_progress.SetPos((int)dbProgress);
		}
		else
		{
			dlg->m_progress.SetPos((int)dbProgress);
			dlg->m_progressText.SetWindowText("复制完成");
			ClearFileHandle();
		}
		LeaveCriticalSection(&g_csForVct);
	}
	CloseHandle(hSourceMap);
	CloseHandle(hDestMap);
	dlg->m_hThreads[nIdx] = NULL;
	BOOL hasThreads = FALSE;
	for (int i = 0; i < dlg->m_hThreadsNum; i++)

	{
		if (dlg->m_hThreads[i] != NULL)
		{
			hasThreads = TRUE;
		}
	}
	delete pParams;
	if (!hasThreads)
	{
		DeleteCriticalSection(&g_csForVct);
	}
	return 0;
}

void ClearFileHandle()
{
	if (g_hSrcFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hSrcFile);
		g_hSrcFile = INVALID_HANDLE_VALUE;
	}
	if (g_hDstFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hDstFile);
		g_hDstFile = INVALID_HANDLE_VALUE;
	}
}

void CFileCopyDlgDlg::OnBnClickedStart()
{
	BOOL hasThreads = FALSE;
	for (int i = 0; i < m_hThreadsNum; i++)
	{
		if (m_hThreads[i] != NULL)
		{
			hasThreads = TRUE;
		}
	}
	if (!hasThreads)
	{

		InitializeCriticalSection(&g_csForVct);
		char sourcePath[MAXBYTE] = {};
		::GetDlgItemText(GetSafeHwnd(), EDT_SOURCE, sourcePath, MAXBYTE);
		char destPath[MAXBYTE] = {};
		::GetDlgItemText(GetSafeHwnd(), EDT_DEST, destPath, MAXBYTE);
		//打开文件
		 g_hSrcFile = CreateFile(sourcePath,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		g_hDstFile = CreateFile(destPath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (g_hSrcFile == INVALID_HANDLE_VALUE || g_hDstFile == INVALID_HANDLE_VALUE)
		{
			ClearFileHandle();
			return;
		}
		
		g_nBytesAllReaede = 0;
		g_nFileSize = 0;
		GetFileSizeEx(g_hSrcFile, (PLARGE_INTEGER)&g_nFileSize);

		GetSystemInfo(&g_si);
		int nAllSplits = g_nFileSize / g_si.dwAllocationGranularity;
		int nSplitSize = nAllSplits / m_hThreadsNum;
		m_progressText.SetWindowText("等待复制");
		for (int i = 0; i < m_hThreadsNum; i++)
		{
			tagThreadsParams* params = new tagThreadsParams(
				{
					this,
					i,
					(LONGLONG)i * nSplitSize * g_si.dwAllocationGranularity,
					i == 3 ? g_nFileSize
					    : ((LONGLONG)(i + (LONGLONG)1)) * nSplitSize * g_si.dwAllocationGranularity,
				}
			);
			m_hThreadsParamsList[i] = params;
			m_hThreads[i] = CreateThread(
				NULL,
				0,              //使用默认的栈大小
				CopyThreadProce, //线程回调函数地址，新线程从这个函数开始执行代码
				params,           //自定参数，会传递给线程回调函数
				0,              //线程立即运行
				NULL            //线程id，不需要
			);
		}

		for (int i = 0; i < m_hThreadsNum; i++)
		{
			if (m_hThreads[i] != NULL)
			{
				CloseHandle(m_hThreads[i]);
			}
		}
	}
	else
	{
		AfxMessageBox("正在复制中");
	}
	return;
}
