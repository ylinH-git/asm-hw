
// RegManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RegManager.h"
#include "RegManagerDlg.h"
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


// CRegManagerDlg 对话框



CRegManagerDlg::CRegManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGMANAGER_DIALOG, pParent)
	, m_curPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CRegManagerDlg::~CRegManagerDlg()
{
	for (std::map<HKEY, PVOID>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) 
	{
		RegCloseKey(it->first);
	}
}


void CRegManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_PATH, m_curPath);
	DDX_Control(pDX, TREE_KEY, m_keyTree);
	DDX_Control(pDX, LIST_VALUE, m_valueList);
}

BEGIN_MESSAGE_MAP(CRegManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(NM_CLICK, TREE_KEY, &CRegManagerDlg::OnClickTreeKey)
ON_NOTIFY(TVN_ITEMCHANGED, TREE_KEY, &CRegManagerDlg::OnItemchangedTreeKey)
ON_NOTIFY(NM_DBLCLK, TREE_KEY, &CRegManagerDlg::OnDblclkTreeKey)
ON_NOTIFY(TVN_ITEMEXPANDING, TREE_KEY, &CRegManagerDlg::OnItemexpandingTreeKey)
ON_NOTIFY(TVN_SELCHANGED, TREE_KEY, &CRegManagerDlg::OnSelchangedTreeKey)
ON_BN_CLICKED(BTN_ADDKEY, &CRegManagerDlg::OnBnClickedAddkey)
ON_BN_CLICKED(BTN_DELKEY, &CRegManagerDlg::OnBnClickedDelkey)
ON_BN_CLICKED(BTN_DELVALUE, &CRegManagerDlg::OnBnClickedDelvalue)
ON_BN_CLICKED(BTN_ADDDWORD, &CRegManagerDlg::OnBnClickedAdddword)
ON_BN_CLICKED(BTN_ADDQWORD, &CRegManagerDlg::OnBnClickedAddqword)
ON_BN_CLICKED(BTN_ADDSTR, &CRegManagerDlg::OnBnClickedAddstr)
ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CRegManagerDlg::OnItemdblclickListValue)
ON_NOTIFY(NM_DBLCLK, LIST_VALUE, &CRegManagerDlg::OnDblclkListValue)
END_MESSAGE_MAP()


// CRegManagerDlg 消息处理程序

void CRegManagerDlg::GetCurValues(const HTREEITEM& curItem)
{
	m_valueList.DeleteAllItems();
	CString parentText;
	HTREEITEM hParent = m_keyTree.GetParentItem(curItem);
	
	while (hParent != NULL)
	{
		parentText = m_keyTree.GetItemText(hParent)+ "\\" + parentText;
		hParent = m_keyTree.GetParentItem(hParent);
	}

	CString curText;
	curText = m_keyTree.GetItemText(curItem);
	m_curHkey = (HKEY)m_keyTree.GetItemData(curItem);

	m_curPath = parentText + curText;
	UpdateData(FALSE);

	HKEY hKey = NULL;
	LONG nRet = RegOpenKey(m_curHkey, NULL, &hKey);

	DWORD dwCntOfValues = 0;
	DWORD dwMaxNameLen = 0;
	DWORD dwMaxDataLen = 0;
	nRet = RegQueryInfoKey(
		hKey,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&dwCntOfValues,
		&dwMaxNameLen,
		&dwMaxDataLen,
		NULL,
		NULL
	);

	char* szName = new char[dwMaxNameLen + 1];
	char* pData = new char[dwMaxDataLen];
	for (size_t nRowIdx = 0; nRowIdx < dwCntOfValues; ++nRowIdx)
	{
		DWORD dwNameLen = dwMaxNameLen + 1;
		DWORD dwType = 0;
		DWORD dwDataLen = dwMaxDataLen;
		nRet = RegEnumValue(
			hKey,
			nRowIdx,
			szName,
			&dwNameLen,
			NULL,
			&dwType,
			(BYTE*)pData,
			&dwDataLen
		);

		SetValue(nRowIdx, szName, dwType, dwDataLen, pData);
	}

	SetListColumnSize();
}

void CRegManagerDlg::SetValue(const size_t& nRowIdx, char* szName, const DWORD& dwType, const DWORD& dwDataLen, char* pData)
{
	CString strData;
	m_valueList.InsertItem(nRowIdx, szName);
	switch (dwType)
	{
	case REG_BINARY: {
		m_valueList.SetItemText(nRowIdx, 1, "REG_BINARY");
		CString formatChar;
		for (int i = 0; i < dwDataLen; i++)
		{
			formatChar.Empty();
			formatChar.Format("%x", pData[i]);
			strData += formatChar;
		}
		break;
	}
	case REG_DWORD:
		m_valueList.SetItemText(nRowIdx, 1, "REG_DWORD");
		strData.Format("%08x", *(DWORD*)pData);
		break;
	case REG_DWORD_BIG_ENDIAN:
		m_valueList.SetItemText(nRowIdx, 1, "REG_DWORD_BIG_ENDIAN");
		strData.Format("%08x", *(DWORD*)pData);
		break;
	case REG_EXPAND_SZ:
		m_valueList.SetItemText(nRowIdx, 1, "REG_EXPAND_SZ");
		strData.Format("%s", pData);
		break;
	case REG_LINK:
		m_valueList.SetItemText(nRowIdx, 1, "REG_LINK");
		strData.Format("%s", pData);
		break;
	case REG_MULTI_SZ:
	{
		m_valueList.SetItemText(nRowIdx, 1, "REG_MULTI_SZ");
		CString formatChar;
		for (int i = 0; i < dwDataLen; i++)
		{
			formatChar.Empty();
			if (pData[i] == '\0')
			{
				formatChar = ", ";
			}
			else
			{
				formatChar.Format("%c", pData[i]);
			}
			strData += formatChar;
		}
		break;
	}
	case REG_QWORD:
	{
		LARGE_INTEGER bInt = *(PLARGE_INTEGER)pData;
		m_valueList.SetItemText(nRowIdx, 1, "REG_QWORD");
		strData.Format("%08x%x", bInt.HighPart, bInt.LowPart);
		break;
	}
	case REG_SZ:
		m_valueList.SetItemText(nRowIdx, 1, "REG_SZ");
		strData.Format("%s", pData);
		break;
	case REG_NONE:
		m_valueList.SetItemText(nRowIdx, 1, "REG_NONE");
		strData = "数值未设置";
		break;
	default:
		break;
	}
	m_valueList.SetItemText(nRowIdx, 2, strData);
}

void CRegManagerDlg::InitKeyTree()
{
	m_keyTree.ModifyStyle(NULL, TVS_LINESATROOT | TVS_HASLINES | TVS_HASBUTTONS);
	auto root = m_keyTree.InsertItem("计算机");
	m_keyTree.SetItemData(root, NULL);
	auto classRoot = m_keyTree.InsertItem("HKEY_CLASSES_ROOT", root);
	m_keyTree.SetItemData(classRoot, (DWORD_PTR)HKEY_CLASSES_ROOT);
	GenerateChildItems(classRoot);
	m_keyList[HKEY_CLASSES_ROOT] = NULL;
	auto currentUser = m_keyTree.InsertItem("HKEY_CURRENT_USER", root);
	m_keyTree.SetItemData(currentUser, (DWORD_PTR)HKEY_CURRENT_USER);
	GenerateChildItems(currentUser);
	m_keyList[HKEY_CURRENT_USER] = NULL;
	auto localMachine = m_keyTree.InsertItem("HKEY_LOCAL_MACHINE", root);
	m_keyTree.SetItemData(localMachine, (DWORD_PTR)HKEY_LOCAL_MACHINE);
	GenerateChildItems(localMachine);
	m_keyList[HKEY_LOCAL_MACHINE] = NULL;
	auto users = m_keyTree.InsertItem("HKEY_USERS", root);
	m_keyTree.SetItemData(users, (DWORD_PTR)HKEY_USERS);
	GenerateChildItems(users);
	m_keyList[HKEY_USERS] = NULL;
	auto currentConfig = m_keyTree.InsertItem("HKEY_CURRENT_CONFIG", root);
	m_keyTree.SetItemData(currentConfig, (DWORD_PTR)HKEY_CURRENT_CONFIG);
	GenerateChildItems(currentConfig);
	m_keyList[HKEY_CURRENT_CONFIG] = NULL;

	m_keyTree.SelectItem(classRoot);
	m_keyTree.SetFocus();
}

void CRegManagerDlg::GetSubKeys(const HTREEITEM& parentItem)
{
	HKEY curKey = (HKEY)m_keyTree.GetItemData(parentItem);
	HKEY hKey = NULL;
	LONG nRet = RegOpenKey(curKey, NULL, &hKey);


	DWORD dwCntOfSubkeys = 0;
	DWORD dwMaxSubkeyLen = 0;
	// 此api可以拿到该键相关的大多数重要信息（几乎所有）
	nRet = RegQueryInfoKey(
		hKey,
		NULL,
		NULL,
		NULL,
		&dwCntOfSubkeys,
		&dwMaxSubkeyLen,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	char* sz = new char[dwMaxSubkeyLen];
	for (size_t i = 0; i < dwCntOfSubkeys; ++i)
	{
		HKEY subKey = NULL;
		//此键用于遍历
		RegEnumKey(hKey, i, sz, dwMaxSubkeyLen);
		RegOpenKey(hKey, sz, &subKey);
		auto subKeyItem = m_keyTree.InsertItem(sz, parentItem);
		m_keyTree.SetItemData(subKeyItem, (DWORD_PTR)subKey);
		GenerateChildItems(subKeyItem);
		m_keyList[subKey] = NULL;
	}
}

void CRegManagerDlg::InitValueList()
{
	m_valueList.SetExtendedStyle(m_valueList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_valueList.InsertColumn(0, "名称");
	m_valueList.InsertColumn(1, "类型");
	m_valueList.InsertColumn(2, "数据");
	SetListColumnSize();
}

void CRegManagerDlg::SetListColumnSize()
{
	m_valueList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_valueList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_valueList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}

BOOL CRegManagerDlg::OnInitDialog()
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
	InitValueList();
	InitKeyTree();
	
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRegManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRegManagerDlg::OnPaint()
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
HCURSOR CRegManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CRegManagerDlg::OnClickTreeKey(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CRegManagerDlg::OnItemchangedTreeKey(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	auto curItem = m_keyTree.GetSelectedItem();
	GetCurValues(curItem);
}


void CRegManagerDlg::OnDblclkTreeKey(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//auto curItem = m_keyTree.GetSelectedItem();
	//m_keyTree.Expand(curItem, TVE_TOGGLE);
}

void CRegManagerDlg::GenerateChildItems(const HTREEITEM& curItem)
{
	if (m_keyTree.ItemHasChildren(curItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_keyTree.GetChildItem(curItem);

		while (hChildItem != NULL)
		{
			hNextItem = m_keyTree.GetNextItem(hChildItem, TVGN_NEXT);
			m_keyTree.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
	else
	{
		HKEY curKey = (HKEY)m_keyTree.GetItemData(curItem);
		HKEY hKey = NULL;
		LONG nRet = RegOpenKey(curKey, NULL, &hKey);

		DWORD dwCntOfSubkeys = 0;
		// 此api可以拿到该键相关的大多数重要信息（几乎所有）
		nRet = RegQueryInfoKey(
			hKey,
			NULL,
			NULL,
			NULL,
			&dwCntOfSubkeys,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
		if (dwCntOfSubkeys > 0)
		{
			m_keyTree.InsertItem("", curItem);
		}
	}
}


void CRegManagerDlg::OnItemexpandingTreeKey(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	auto curItem = pNMTreeView->itemNew.hItem;
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	if (curItemData != NULL)
	{
		if (!(pNMTreeView->itemNew.state & TVIS_EXPANDED)) {
			GenerateChildItems(curItem);
			GetSubKeys(curItem);
		}
	}
}


void CRegManagerDlg::OnSelchangedTreeKey(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	auto curItem = m_keyTree.GetSelectedItem();
	GetCurValues(curItem);
}


void CRegManagerDlg::OnBnClickedAddkey()
{
	// TODO: 在此添加控件通知处理程序代码
	auto curItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	CAddKeyDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		HKEY hKey = NULL;
		LONG nRet = RegCreateKey(
			curItemData,
			dlg.m_name.GetString(),
			&hKey
		);
		if (nRet != ERROR_SUCCESS)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				nRet,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			AfxMessageBox((LPTSTR)lpMsgBuf);
		}
		else
		{
			auto subKeyItem = m_keyTree.InsertItem(dlg.m_name, curItem);
			m_keyTree.SetItemData(subKeyItem, (DWORD_PTR)hKey);
			GenerateChildItems(subKeyItem);
			m_keyList[hKey] = NULL;
		}
	}
}


void CRegManagerDlg::OnBnClickedDelkey()
{
	// TODO: 在此添加控件通知处理程序代码
	auto curItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	LONG nRet = RegDeleteTree(curItemData, NULL);
	if (nRet != ERROR_SUCCESS)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nRet,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		AfxMessageBox((LPTSTR)lpMsgBuf);
	}
	else
	{
		m_keyTree.DeleteItem(curItem);
		std::map<HKEY, PVOID>::iterator it;
		it = m_keyList.find(curItemData);
		if (it != m_keyList.end())
			m_keyList.erase(it);
		RegCloseKey(curItemData);
	}
}


//void CRegManagerDlg::OnBnClickedAddvalue()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CRegManagerDlg::OnBnClickedDelvalue()
{
	// TODO: 在此添加控件通知处理程序代码
	
	auto curItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	int curValueIdx = m_valueList.GetSelectionMark();
	if (curValueIdx == -1)
	{
		return;
	}
	CString curValue = m_valueList.GetItemText(curValueIdx, 0);
	LONG nRet = RegDeleteValue(curItemData, curValue.GetString());
	if (nRet != ERROR_SUCCESS)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nRet,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		AfxMessageBox((LPTSTR)lpMsgBuf);
	}
	else
	{
		m_valueList.DeleteItem(curValueIdx);
	}
}


void CRegManagerDlg::OnBnClickedAdddword()
{
	// TODO: 在此添加控件通知处理程序代码
	auto curItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	if (curItemData == NULL)
	{
		return;
	}
	CAddDwordDlg dlg;
	SetDwordValue(dlg, curItemData, curItem);
}

void CRegManagerDlg::SetDwordValue(CAddDwordDlg& dlg, const HKEY& curItemData, HTREEITEM& curItem)
{
	if (dlg.DoModal() == IDOK)
	{
		DWORD value;
		sscanf_s(dlg.m_value.GetString(), "%08x", &value);
		LONG nRet = RegSetValueEx(
			curItemData,
			dlg.m_name.GetString(),
			0,
			REG_DWORD,
			(BYTE*)&value,
			sizeof(DWORD)
		);
		if (nRet != ERROR_SUCCESS)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				nRet,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			AfxMessageBox((LPTSTR)lpMsgBuf);
		}
		else
		{
			GetCurValues(curItem);
		}
	}
}


void CRegManagerDlg::OnBnClickedAddqword()
{
	// TODO: 在此添加控件通知处理程序代码
	auto curItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	if (curItemData == NULL)
	{
		return;
	}
	CAddQwordDlg dlg;
	SetQwordValue(dlg, curItemData, curItem);
}

void CRegManagerDlg::SetQwordValue(CAddQwordDlg& dlg, const HKEY& curItemData, HTREEITEM& curItem)
{
	if (dlg.DoModal() == IDOK)
	{
		LARGE_INTEGER value;
		int valueLen = dlg.m_value.GetLength();
		if (valueLen > 8)
		{
			int restLen = valueLen - 8;
			char* highPartStr = new char[restLen + 1];
			char* lowPartStr = nullptr;
			char* szBuf = dlg.m_value.GetBuffer();
			for (int i = 0; i < restLen; i++)
			{
				highPartStr[i] = szBuf[i];
			}
			highPartStr[restLen] = '\0';
			lowPartStr = &(szBuf[restLen]);
			sscanf_s(highPartStr, "%x", &value.HighPart);
			sscanf_s(lowPartStr, "%x", &value.LowPart);
			delete[] highPartStr;
		}
		else
		{
			sscanf_s(dlg.m_value.GetString(), "%08x", &value.LowPart);
		}
		LONG nRet = RegSetValueEx(
			curItemData,
			dlg.m_name.GetString(),
			0,
			REG_QWORD,
			(BYTE*)&value,
			sizeof(QWORD)
		);
		if (nRet != ERROR_SUCCESS)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				nRet,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			AfxMessageBox((LPTSTR)lpMsgBuf);
		}
		else
		{
			GetCurValues(curItem);
		}
	}
}


void CRegManagerDlg::OnBnClickedAddstr()
{
	auto curItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curItem);
	if (curItemData == NULL)
	{
		return;
	}
	CAddStrDlg dlg;
	SetStrValue(dlg, curItemData, curItem);
}

void CRegManagerDlg::SetStrValue(CAddStrDlg& dlg, const HKEY& curItemData, HTREEITEM& curItem)
{
	if (dlg.DoModal() == IDOK)
	{
		LONG nRet = RegSetValueEx(
			curItemData,
			dlg.m_name.GetString(),
			0,
			REG_SZ,
			(BYTE*)dlg.m_value.GetBuffer(),
			dlg.m_value.GetLength()
		);
		if (nRet != ERROR_SUCCESS)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				nRet,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			AfxMessageBox((LPTSTR)lpMsgBuf);
		}
		else
		{
			GetCurValues(curItem);
		}
	}
}


void CRegManagerDlg::OnItemdblclickListValue(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CRegManagerDlg::OnDblclkListValue(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	auto curTreeItem = m_keyTree.GetSelectedItem();
	HKEY curItemData = (HKEY)m_keyTree.GetItemData(curTreeItem);
	auto curItem = m_valueList.GetSelectionMark();
	CString valueName = m_valueList.GetItemText(curItem, 0);
	CString valueType = m_valueList.GetItemText(curItem, 1);
	CString valueData = m_valueList.GetItemText(curItem, 2);

	if (valueType == "REG_SZ")
	{
		CAddStrDlg dlg;
		dlg.SetDlg(valueName, valueData);
		SetStrValue(dlg, curItemData, curTreeItem);
	} else if (valueType == "REG_DWORD")
	{
		CAddDwordDlg dlg;
		dlg.SetDlg(valueName, valueData);
		SetDwordValue(dlg, curItemData, curTreeItem);
	} else if (valueType == "REG_QWORD")
	{
		CAddQwordDlg dlg;
		dlg.SetDlg(valueName, valueData);
		SetQwordValue(dlg, curItemData, curTreeItem);
	}
}
