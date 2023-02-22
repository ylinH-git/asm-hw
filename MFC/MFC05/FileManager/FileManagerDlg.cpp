
// FileManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileManager.h"
#include "FileManagerDlg.h"
#include "afxdialogex.h"
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


// CFileManagerDlg 对话框



CFileManagerDlg::CFileManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFileManagerDlg::~CFileManagerDlg()
{
	for (auto mapLt = m_fileMap.begin(); mapLt != m_fileMap.end(); mapLt++)
	{
		delete mapLt->second;
	}
}

void CFileManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, COMBO_FILESTACK, m_addrBox);
	DDX_Control(pDX, TCTR_FILETREE, m_fileTree);
	DDX_Control(pDX, LCTR_FILELIST, m_fileList);
}

BEGIN_MESSAGE_MAP(CFileManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_BACK, &CFileManagerDlg::OnBnClickedBack)
	ON_BN_CLICKED(BTN_NEXT, &CFileManagerDlg::OnBnClickedNext)
	ON_BN_CLICKED(BTN_GO, &CFileManagerDlg::OnBnClickedGo)
	ON_NOTIFY(TVN_SELCHANGED, TCTR_FILETREE, &CFileManagerDlg::OnSelchangedTctrFiletree)
	ON_CBN_SELCHANGE(COMBO_FILESTACK, &CFileManagerDlg::OnSelchangeComboFilestack)
END_MESSAGE_MAP()


// CFileManagerDlg 消息处理程序

BOOL CFileManagerDlg::OnInitDialog()
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
	CreateFileTree();
	InitList();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileManagerDlg::InitList()
{
	m_fileList.SetExtendedStyle(m_fileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_fileList.InsertColumn(0, "名称");
	m_fileList.InsertColumn(1, "创建日期");
	m_fileList.InsertColumn(2, "修改日期");
	m_fileList.InsertColumn(3, "文件大小");
	m_fileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_fileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_fileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_fileList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}

void CFileManagerDlg::CreateFileTree()
{
	m_fileTree.ModifyStyle(NULL, TVS_LINESATROOT | TVS_HASLINES | TVS_HASBUTTONS);

	GetDrivesFiles();
	GetDeskFiles();
}

void CFileManagerDlg::GetDrivesFiles()
{
	CArray<CString> aryPath;
	GetSytstemDrive(aryPath);
	tagFileInfo* systemInfo = new tagFileInfo();
	systemInfo->m_fPath = "myComputer";
	systemInfo->m_isDir = TRUE;
	m_fileMap[systemInfo->m_fPath] = systemInfo;
	auto systemTree = m_fileTree.InsertItem("此电脑");
	m_fileTree.SetItemData(systemTree, (DWORD_PTR)systemInfo);
	for (int i = 0; i < aryPath.GetSize(); i++)
	{
		auto driveTree = m_fileTree.InsertItem(aryPath[i], systemTree);
		tagFileInfo* driveInfo = new tagFileInfo();
		char drivePath[3] = {};
		drivePath[0] = aryPath[i].GetAt(0);
		drivePath[1] = aryPath[i].GetAt(1);
		driveInfo->m_fPath = drivePath;
		driveInfo->m_isDir = TRUE;
		m_fileTree.SetItemData(driveTree, (DWORD_PTR)driveInfo);
		GenerateFileTree(aryPath[i] + "*", driveTree, 1);
	}
}

void CFileManagerDlg::GetDeskFiles()
{
	TCHAR deskPath[_MAX_PATH];
	SHGetSpecialFolderPath(GetSafeHwnd(), deskPath, CSIDL_DESKTOP, 0);
	GenerateFileTree(deskPath);
}

void CFileManagerDlg::GenerateFileTree(CString targetPath, HTREEITEM parentItem, int nLayer)
{
	CFileFind finder;
	CList<CString> stack;
	CList<HTREEITEM> parentStack;
	CList<int> layerStack;
	parentStack.AddTail((HTREEITEM)parentItem);
	stack.AddTail(targetPath);
	layerStack.AddTail(0);
	while (!stack.IsEmpty())
	{
		auto curPath = stack.GetTail();
		stack.RemoveTail();
		auto parent = parentStack.GetTail();
		parentStack.RemoveTail();
		auto curLayer = layerStack.GetTail();
		layerStack.RemoveTail();
		BOOL bResult = finder.FindFile(curPath);
		while (bResult)
		{
			bResult = finder.FindNextFile();
			CString curPath = finder.GetFilePath();
			if (m_fileMap[curPath] != nullptr)
			{
				break;
			}
			auto curFileInfo = new tagFileInfo({});
			curFileInfo->m_fPath = curPath;
			curFileInfo->m_isDir = finder.IsDirectory();
			auto curFileName = finder.GetFileName();
			if (curFileName != "." && curFileName != "..")
			{
				auto curItem = m_fileTree.InsertItem(curFileName, parent);
				curFileInfo->m_fItem = curItem;
				m_fileTree.SetItemData(curItem, (DWORD_PTR)curFileInfo); 
				m_fileMap[curFileInfo->m_fPath] = curFileInfo;
				if (curFileInfo->m_isDir && curLayer < nLayer)
				{
					stack.AddTail(curFileInfo->m_fPath + "\\*");
					parentStack.AddTail(curItem);
					layerStack.AddTail(curLayer + 1);
				}
			}
		}
	}
	finder.Close();
}

void CFileManagerDlg::GetSytstemDrive(CArray<CString>& aryPath)
{
	DWORD dwLen = GetLogicalDriveStrings(0, NULL);	//获取系统盘符字符串长度
	char* pszDriver = new char[dwLen];	//建构字符数组
	GetLogicalDriveStrings(dwLen, pszDriver);	//获取系统盘符字符串
	char* pDriver = pszDriver;	//定义一个临时指针
	while (*pDriver != '\0')	//遍历字符串
	{
		CString sPath =pDriver;
		aryPath.Add(sPath);
		pDriver += strlen(pDriver) + 1;	//定义到下一个字符串，加1是为了跳过'\0'字符
	}

	delete[] pszDriver;	//释放字符数组
}

void CFileManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileManagerDlg::OnPaint()
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
HCURSOR CFileManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileManagerDlg::OnBnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_addrBox.GetCurSel();
	if (nIndex > 0)
	{
		GoPage(nIndex);
	}
}

void CFileManagerDlg::GoPage(int& nIndex, BOOL bBack)
{
	m_fileList.DeleteAllItems();
	if (bBack)
	{
		m_addrBox.SetCurSel(--nIndex);
	}
	else
	{
		m_addrBox.SetCurSel(++nIndex);
	}
	CString curPath;
	m_addrBox.GetLBText(nIndex, curPath);
	auto curItemData = m_fileMap[curPath];
	if (curItemData != nullptr)
	{
		RefreshFileList(curItemData, curItemData->m_fItem);
	}
}


void CFileManagerDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_addrBox.GetCurSel();
	int nCount = m_addrBox.GetCount();
	if (nIndex != -1 && nIndex < nCount - 1)
	{
		GoPage(nIndex, FALSE);
	}
}


void CFileManagerDlg::OnBnClickedGo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileList.DeleteAllItems();
	CString curPath;
	m_addrBox.GetWindowText(curPath);
	auto curFileInfo = m_fileMap[curPath];
	if (curFileInfo != nullptr)
	{
		RefreshFileList(curFileInfo, curFileInfo->m_fItem);
	}
	else
	{
		GeneratorFileList(curPath);
	}
}

void CFileManagerDlg::GeneratorFileList(CString& curPath)
{
	CFileFind finder;
	auto bResult = finder.FindFile(curPath + "\\*");
	int nRowIdx = 0;
	while (bResult)
	{
		bResult = finder.FindNextFile();
		m_fileList.InsertItem(nRowIdx, finder.GetFileName());
		CTime creationTime;
		finder.GetCreationTime(creationTime);
		CTime lastWriteTime;
		finder.GetLastWriteTime(lastWriteTime);
		m_fileList.SetItemText(nRowIdx, 1, creationTime.Format("%Y-%m-%d %H:%M:%S"));
		m_fileList.SetItemText(nRowIdx, 2, lastWriteTime.Format("%Y-%m-%d %H:%M:%S"));
		CString sLength = std::to_string(finder.GetLength()).c_str();
		m_fileList.SetItemText(nRowIdx, 3, sLength + " kb");
		++nRowIdx;
	}
	finder.Close();
	m_fileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_fileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_fileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_fileList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}

void CFileManagerDlg::OnSelchangedTctrFiletree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	auto curItem = m_fileTree.GetSelectedItem();
	m_fileList.DeleteAllItems();
	tagFileInfo* curItemDataPtr = (tagFileInfo*)m_fileTree.GetItemData(curItem);
	if (curItemDataPtr->m_fPath == "myComputer" || curItemDataPtr == nullptr)
	{
		return;
	}

	RefreshFileList(curItemDataPtr, curItem);
	m_addrBox.InsertString(0, curItemDataPtr->m_fPath);
	m_addrBox.SetCurSel(0);
	*pResult = 0;
}

void CFileManagerDlg::RefreshFileList(tagFileInfo* curItemDataPtr, const HTREEITEM& curItem)
{
	if (curItemDataPtr->m_isDir)
	{
		GenerateFileTree(curItemDataPtr->m_fPath + "\\*", curItem);
	}
	GeneratorFileList(curItemDataPtr->m_fPath);
}


void CFileManagerDlg::OnSelchangeComboFilestack()
{
	m_fileList.DeleteAllItems();
	int nIndex = m_addrBox.GetCurSel();
	CString curPath;
	m_addrBox.GetLBText(nIndex, curPath);
	auto curItemData = m_fileMap[curPath];
	if (curItemData != nullptr)
	{
		RefreshFileList(curItemData, curItemData->m_fItem);
	}
}

