
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

CpetoolDlg::~CpetoolDlg()
{
	if (theApp.m_pFile) 
	{
		fclose(theApp.m_pFile);
		theApp.m_pFile = nullptr;
	}

	if (theApp.m_dataDirectoris)
	{
		delete[] theApp.m_dataDirectoris;
		theApp.m_dataDirectoris = nullptr;
	}

	if (theApp.m_sectionHeaders)
	{
		delete[] theApp.m_sectionHeaders;
		theApp.m_sectionHeaders = nullptr;
	}

	if (theApp.m_importDescriptors)
	{
		delete[] theApp.m_importDescriptors;
		theApp.m_importDescriptors = nullptr;
	}

	if (theApp.m_importDllNameFAs)
	{
		delete[] theApp.m_importDllNameFAs;
		theApp.m_importDllNameFAs = nullptr;
	}
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
	InitFileInfoDlg();
	InitDosHeaderDlg();
	InitNtHeadersDlg();
	InitFileHeaderDlg();
	InitOptionalHeaderDlg();
	InitDataDirectoriesDlg();
	InitSectionHeaderDlg();
	InitImportDirectoryDlg();
	InitAddrConvertDlg();

	CRect rc;
	GetClientRect(&rc);

	rc.left += 340;
	rc.top += 53;

	m_fileInfoDlg.MoveWindow(&rc);
	m_dosHeaderDlg.MoveWindow(&rc);
	m_ntHeaderDlg.MoveWindow(&rc);
	m_fileHeaderDlg.MoveWindow(&rc);
	m_optionalHeaderDlg.MoveWindow(&rc);
	m_dataDirectoriesDlg.MoveWindow(&rc);
	m_sectionHeaderDlg.MoveWindow(&rc);
	m_addrConverDlg.MoveWindow(&rc);
	m_importDirectoryDlg.MoveWindow(&rc);
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
	m_peFileInfo = m_peTree.InsertItem("File：" + fileName);
	m_dosHeader = m_peTree.InsertItem("Dos Header", m_peFileInfo);
	m_ntHeader = m_peTree.InsertItem("Nt Header", m_peFileInfo);
	m_fileHeader = m_peTree.InsertItem("File Header", m_ntHeader);
	m_optionalHeader = m_peTree.InsertItem("Optional Header", m_ntHeader);
	m_dataDirectories =  m_peTree.InsertItem("Data Direction [x]", m_optionalHeader);
	m_sectionHeaders = m_peTree.InsertItem("Section Headers [X]", m_peFileInfo);
	m_importDirectory = m_peTree.InsertItem("Import Directory", m_peFileInfo);
	m_resDirectory = m_peTree.InsertItem("Resource Directory", m_peFileInfo);
	m_addrConverter =  m_peTree.InsertItem("Address Converter", m_peFileInfo);
	m_depWalker =  m_peTree.InsertItem("Dependency Walker", m_peFileInfo);
	m_hexEditor = m_peTree.InsertItem("Hex Editor", m_peFileInfo);
	m_identifier = m_peTree.InsertItem("Identifier", m_peFileInfo);
	m_importAdder = m_peTree.InsertItem("Import Adder", m_peFileInfo);
	m_quickDisasm = m_peTree.InsertItem("Quick Disassembler", m_peFileInfo);
	m_rebuilder = m_peTree.InsertItem("Rebuilder", m_peFileInfo);
	m_resEditor = m_peTree.InsertItem("Resource Editor", m_peFileInfo);
	m_upx = m_peTree.InsertItem("UPX Utility", m_peFileInfo);

	HideAllDlg();
	m_fileInfoDlg.ShowWindow(SW_SHOW);

	m_fileInfoDlg.OnInitDialog();
	m_dosHeaderDlg.OnInitDialog();
	m_ntHeaderDlg.OnInitDialog();
	m_fileHeaderDlg.OnInitDialog();
	m_optionalHeaderDlg.OnInitDialog();
	m_dataDirectoriesDlg.OnInitDialog();
	m_sectionHeaderDlg.OnInitDialog();
	m_importDirectoryDlg.OnInitDialog();

}

void CpetoolDlg::GetDosStruct()
{
	int dosHeaderLen = sizeof(IMAGE_DOS_HEADER);
	RtlZeroMemory(&theApp.m_dosHeaderBuf, dosHeaderLen);
	fread(&theApp.m_dosHeaderBuf, 1, dosHeaderLen, theApp.m_pFile);
	theApp.m_ntOffset = theApp.m_dosHeaderBuf.e_lfanew;
}

void CpetoolDlg::GetNtStruct()
{
	int ntHeaderLen = sizeof(IMAGE_NT_HEADERS);
	fseek(theApp.m_pFile, theApp.m_ntOffset, SEEK_SET);
	RtlZeroMemory(&theApp.m_ntHeader, ntHeaderLen);
	fread(&theApp.m_ntHeader, 1, ntHeaderLen, theApp.m_pFile);
}

void CpetoolDlg::GetFileHStruct()
{
	theApp.m_fileHeader = theApp.m_ntHeader.FileHeader;
	if (theApp.m_fileHeader.Machine == IMAGE_FILE_MACHINE_I386)
	{
		theApp.isx86 = true;
	}
	else
	{
		theApp.isx86 = false;
	}
}

void CpetoolDlg::GetOptionalStruct()
{
	theApp.m_optionalOffset = theApp.m_ntOffset + sizeof(theApp.m_ntHeader.Signature) + sizeof(theApp.m_ntHeader.FileHeader);
	fseek(theApp.m_pFile, theApp.m_optionalOffset, SEEK_SET);
	if (theApp.isx86)
	{
		int op32Len = sizeof(IMAGE_OPTIONAL_HEADER32);
		int op64Len = sizeof(IMAGE_OPTIONAL_HEADER64);
		RtlZeroMemory(&theApp.m_optional64Header, op64Len);
		fread(&theApp.m_optional32Header, 1, op32Len, theApp.m_pFile);
	}
	else
	{
		int op32Len = sizeof(IMAGE_OPTIONAL_HEADER32);
		int op64Len = sizeof(IMAGE_OPTIONAL_HEADER64);
		RtlZeroMemory(&theApp.m_optional32Header, op32Len);
		fread(&theApp.m_optional64Header, 1, op64Len, theApp.m_pFile);
	}
}

void CpetoolDlg::GetDataDirStruct()
{
	if (theApp.isx86)
	{
		theApp.m_dataDirectoryLen = theApp.m_optional32Header.NumberOfRvaAndSizes;
		theApp.m_dataDirectoryOffset = theApp.m_optionalOffset + 96;
	}
	else
	{
		theApp.m_dataDirectoryLen = theApp.m_optional64Header.NumberOfRvaAndSizes;
		theApp.m_dataDirectoryOffset = theApp.m_optionalOffset + 112;
	}
	fseek(theApp.m_pFile, theApp.m_dataDirectoryOffset, SEEK_SET);
	theApp.m_dataDirectoris = new IMAGE_DATA_DIRECTORY[theApp.m_dataDirectoryLen];
	fread(theApp.m_dataDirectoris, 1, theApp.m_dataDirectoryLen * sizeof(IMAGE_DATA_DIRECTORY), theApp.m_pFile);
}

void CpetoolDlg::GetSectionHeaders()
{
	theApp.m_sectionHeaderOffset = theApp.m_optionalOffset + theApp.m_fileHeader.SizeOfOptionalHeader;
	fseek(theApp.m_pFile, theApp.m_sectionHeaderOffset, SEEK_SET);
	theApp.m_sectionHeaderLen = theApp.m_fileHeader.NumberOfSections;
	theApp.m_sectionHeaders = new IMAGE_SECTION_HEADER[theApp.m_sectionHeaderLen];
	fread(theApp.m_sectionHeaders, 1, theApp.m_sectionHeaderLen * sizeof(IMAGE_SECTION_HEADER), theApp.m_pFile);
}

void CpetoolDlg::GetImportDirectory()
{
	theApp.m_importDescriptorOffset = theApp.m_dataDirectoris[1].VirtualAddress;
	theApp.m_importDescriptorFA = theApp.GetRVAtoFA(theApp.m_importDescriptorOffset);
	theApp.m_importDescriptorLen = 0;
	fseek(theApp.m_pFile, theApp.m_importDescriptorFA, SEEK_SET);
	int importStructLen = sizeof(IMAGE_IMPORT_DESCRIPTOR);
	char* tempImport = new char[importStructLen];
	char* allZero = new char[importStructLen];
	RtlZeroMemory(allZero, importStructLen);
	while (1)
	{
		fread(tempImport, 1,importStructLen, theApp.m_pFile);
		if (memcmp(tempImport, allZero, importStructLen) == 0)
		{
			break;
		}
		theApp.m_importDescriptorLen++;
	}

	theApp.m_importDescriptors = new IMAGE_IMPORT_DESCRIPTOR[theApp.m_importDescriptorLen];
	theApp.m_importDllNameFAs = new ULONG[theApp.m_importDescriptorLen];
	fseek(theApp.m_pFile, theApp.m_importDescriptorFA, SEEK_SET);
	fread(theApp.m_importDescriptors, 1, importStructLen * theApp.m_importDescriptorLen, theApp.m_pFile);

	delete[] tempImport;
	delete[] allZero;
}

void CpetoolDlg::HideAllDlg()
{
	m_fileInfoDlg.ShowWindow(SW_HIDE);
	m_dosHeaderDlg.ShowWindow(SW_HIDE);
	m_ntHeaderDlg.ShowWindow(SW_HIDE);
	m_fileHeaderDlg.ShowWindow(SW_HIDE);
	m_optionalHeaderDlg.ShowWindow(SW_HIDE);
	m_dataDirectoriesDlg.ShowWindow(SW_HIDE);
	m_sectionHeaderDlg.ShowWindow(SW_HIDE);
	m_importDirectoryDlg.ShowWindow(SW_HIDE);
	m_addrConverDlg.ShowWindow(SW_HIDE);
}



void CpetoolDlg::OnBnClickedOpen()
{
	UpdateData(TRUE);

	if (!m_filePath.IsEmpty())
	{
		if (theApp.m_pFile)
		{
			fclose(theApp.m_pFile);
		}
		bool bRet = fopen_s(&theApp.m_pFile, m_filePath.GetString(), "rb");
		if (!bRet)
		{
			GetDosStruct();
			GetNtStruct();
			GetFileHStruct();
			GetOptionalStruct();
			GetDataDirStruct();
			GetSectionHeaders();
			GetImportDirectory();
			if (!m_bIsInited) {
				CreatePeTree(m_filePath);
				m_bIsInited = true;
			}
			else {
				m_peTree.DeleteAllItems();
				CreatePeTree(m_filePath);
			}
		}
		else
		{
			AfxMessageBox("打开文件失败");
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

	HideAllDlg();

	if (pNMTreeView->itemNew.hItem == m_peFileInfo)
	{
		m_fileInfoDlg.ShowWindow(SW_SHOW);
		return;
	}


	if (pNMTreeView->itemNew.hItem == m_dosHeader)
	{
		m_dosHeaderDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_ntHeader)
	{
		m_ntHeaderDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_fileHeader)
	{
		m_fileHeaderDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_optionalHeader)
	{
		m_optionalHeaderDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_dataDirectories)
	{
		m_dataDirectoriesDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_sectionHeaders)
	{
		m_sectionHeaderDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_importDirectory)
	{
		m_importDirectoryDlg.ShowWindow(SW_SHOW);
		return;
	}

	if (pNMTreeView->itemNew.hItem == m_addrConverter)
	{
		m_addrConverDlg.ShowWindow(SW_SHOW);
		return;
	}
}

void CpetoolDlg::InitFileInfoDlg()
{
	m_fileInfoDlg.Create(DLG_FILE_INFO, this);
}

void CpetoolDlg::InitDosHeaderDlg()
{
	m_dosHeaderDlg.Create(DLG_DOS_HEADER, this);
}

void CpetoolDlg::InitNtHeadersDlg()
{
	m_ntHeaderDlg.Create(DLG_NT_HEADER, this);
}

void CpetoolDlg::InitFileHeaderDlg()
{
	m_fileHeaderDlg.Create(DLG_FILE_HEADER, this);
}

void CpetoolDlg::InitOptionalHeaderDlg()
{
	m_optionalHeaderDlg.Create(DLG_OPTIONAL_HEADER, this);
}

void CpetoolDlg::InitDataDirectoriesDlg()
{
	m_dataDirectoriesDlg.Create(DLG_DATA_DIRECTORIES, this);
}

void CpetoolDlg::InitSectionHeaderDlg()
{
	m_sectionHeaderDlg.Create(DLG_SECTION_HEADER, this);
}

void CpetoolDlg::InitAddrConvertDlg()
{
	m_importDirectoryDlg.Create(DLG_IMPORT_DIRECTORY, this);
}

void CpetoolDlg::InitImportDirectoryDlg()
{
	m_addrConverDlg.Create(DLG_ADDR_CONVERT, this);
}
