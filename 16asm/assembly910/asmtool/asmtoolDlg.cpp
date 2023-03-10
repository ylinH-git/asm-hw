
// asmtoolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "asmtool.h"
#include "asmtoolDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "disasm_lib.h"

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


// CasmtoolDlg 对话框



CasmtoolDlg::CasmtoolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASMTOOL_DIALOG, pParent)
	, m_disasmText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CasmtoolDlg::~CasmtoolDlg()
{
	if (m_asmBuf != nullptr)
	{
		delete[] m_asmBuf;
		m_asmBuf = nullptr;
	}
}

void CasmtoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, FILES_EDIT, m_fileEdit);
	DDX_Text(pDX, EDIT_ASM, m_disasmText);
}

BEGIN_MESSAGE_MAP(CasmtoolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_IMPORT, &CasmtoolDlg::OnBnClickedImport)
	ON_BN_CLICKED(BTN_DISASM, &CasmtoolDlg::OnBnClickedDisasm)
END_MESSAGE_MAP()


// CasmtoolDlg 消息处理程序

BOOL CasmtoolDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CasmtoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CasmtoolDlg::OnPaint()
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
HCURSOR CasmtoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CasmtoolDlg::OnBnClickedImport()
{
	CString filePath;
	m_fileEdit.GetWindowText(filePath);
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if (!in.is_open())
	{
		AfxMessageBox("文件导入失败");
		return;
	}
	
	long lFileStart = in.tellg();
	in.seekg(0, std::ios::end);
	long lFileEnd = in.tellg();

	m_fileSize = lFileEnd - lFileStart;

	in.seekg(0, 0);
	m_asmBuf = new char[m_fileSize];

	in.read(m_asmBuf, m_fileSize);
	AfxMessageBox("导入成功，请进行反汇编");
	in.close();
	
}


void CasmtoolDlg::OnBnClickedDisasm()
{
	if (m_asmBuf == nullptr)
	{
		AfxMessageBox("没有需要解析的文件，请重新选择");
	}
	m_disasmText = "";
	int nLen = 0;
	int nCurrentPos = 0;
	std::string disassemlyText;
	while (true)
	{
		int restSize = m_fileSize - nCurrentPos;
		if (restSize <= 0)
		{
			break;
		}
		int nRet = Disassembly((unsigned char*)m_asmBuf + nCurrentPos, restSize, disassemlyText);
		nCurrentPos += nRet;
		if (nRet == 0)
		{
			break;
		}
		else
		{
			m_disasmText += (disassemlyText += "\r\n").c_str();
			disassemlyText = "";
			UpdateData(FALSE);
		}
	}
	delete[] m_asmBuf;
	m_asmBuf = nullptr;
}
