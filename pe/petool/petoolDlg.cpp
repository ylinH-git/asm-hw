
// petoolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "petool.h"
#include "petoolDlg.h"
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


// CpetoolDlg 对话框



CpetoolDlg::CpetoolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PETOOL_DIALOG, pParent)
	, m_filePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpetoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TREE_PE, m_peTree);
	DDX_Text(pDX, FILE_BROWSE, m_filePath);
}

BEGIN_MESSAGE_MAP(CpetoolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_OPEN, &CpetoolDlg::OnBnClickedOpen)
	ON_BN_CLICKED(BTN_SAVE, &CpetoolDlg::OnBnClickedSave)
	ON_NOTIFY(TVN_SELCHANGED, TREE_PE, &CpetoolDlg::OnSelchangedTreePe)
END_MESSAGE_MAP()


// CpetoolDlg 消息处理程序

BOOL CpetoolDlg::OnInitDialog()
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

void CpetoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpetoolDlg::OnPaint()
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
HCURSOR CpetoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpetoolDlg::CreatePeTree(CString fileName)
{
	m_peTree.ModifyStyle(NULL, TVS_LINESATROOT | TVS_HASLINES | TVS_HASBUTTONS);
	auto peFileInfo = m_peTree.InsertItem("File：" + fileName);
	m_peTree.InsertItem("Dos Header", peFileInfo);
	auto ntHeader = m_peTree.InsertItem("Nt Header", peFileInfo);
	m_peTree.InsertItem("File Header", ntHeader);
	auto optionalHeader = m_peTree.InsertItem("Optional Header", ntHeader);
	m_peTree.InsertItem("Data Direction [x]", optionalHeader);
	m_peTree.InsertItem("Import Directory]", peFileInfo);
	m_peTree.InsertItem("Resource Directory", peFileInfo);
	m_peTree.InsertItem("Address Converter", peFileInfo);
	m_peTree.InsertItem("Dependency Walker", peFileInfo);
	m_peTree.InsertItem("Hex Editor", peFileInfo);
	m_peTree.InsertItem("Identifier", peFileInfo);
	m_peTree.InsertItem("Import Adder", peFileInfo);
	m_peTree.InsertItem("Quick Disassembler", peFileInfo);
	m_peTree.InsertItem("Rebuilder", peFileInfo);
	m_peTree.InsertItem("Resource Editor", peFileInfo);
	m_peTree.InsertItem("UPX Utility", peFileInfo);
}



void CpetoolDlg::OnBnClickedOpen()
{
	UpdateData(TRUE);
	if (!m_filePath.IsEmpty())
	{
		if (!m_bIsInited) {
			CreatePeTree(m_filePath);
			m_bIsInited = true;
		}
		else {
			AfxMessageBox("刷新树");
		}
	}
	else
	{
		AfxMessageBox("未选择文件");
	}
}


void CpetoolDlg::OnBnClickedSave()
{
	AfxMessageBox("暂未支持");
}


void CpetoolDlg::OnSelchangedTreePe(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	AfxMessageBox(pNMTreeView->itemNew.pszText);
	//int nIdx = m_tab.GetCurSel();
	//m_generalDlg.ShowWindow(SW_HIDE);
	//m_styleDlg.ShowWindow(SW_HIDE);
	//m_winDlg.ShowWindow(SW_HIDE);
	//m_classDlg.ShowWindow(SW_HIDE);
	//m_prcessDlg.ShowWindow(SW_HIDE);

	//switch (nIdx)
	//{
	//case 0:
	//	m_generalDlg.ShowWindow(SW_SHOW);
	//	break;
	//case 1:
	//	m_styleDlg.ShowWindow(SW_SHOW);
	//	break;
	//case 2:
	//	m_winDlg.ShowWindow(SW_SHOW);
	//	break;
	//case 3:
	//	m_classDlg.ShowWindow(SW_SHOW);
	//	break;
	//case 4:
	//	m_prcessDlg.ShowWindow(SW_SHOW);
	//	break;
	//default:
	//	break;
	//}

}
