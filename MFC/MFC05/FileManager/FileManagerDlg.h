
// FileManagerDlg.h: 头文件
//

#pragma once
#include <memory>
#include <map>

struct tagFileInfo {
	CString m_fPath;
	BOOL m_isDir = FALSE;
	HTREEITEM m_fItem;
};
//using FileInfoPtr = std::shared_ptr<tagFileInfo>;

// CFileManagerDlg 对话框
class CFileManagerDlg : public CDialogEx
{
// 构造
public:
	CFileManagerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CFileManagerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void InitList();
	void CreateFileTree();
	void GetDrivesFiles();
	void GetDeskFiles();
	void GenerateFileTree(CString targetPath, HTREEITEM parentItem = NULL, int nLayer = 2);
	void GetSytstemDrive(CArray<CString>& aryPath);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBack();
	void GoPage(int& nIndex, BOOL bBack = TRUE);
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedGo();
	void GeneratorFileList(CString& curPath);
	afx_msg void OnSelchangedTctrFiletree(NMHDR* pNMHDR, LRESULT* pResult);
	void RefreshFileList(tagFileInfo* curItemDataPtr, const HTREEITEM& curItem);
	CComboBox m_addrBox;
	CTreeCtrl m_fileTree;
	std::map<CString, tagFileInfo*> m_fileMap;
	CListCtrl m_fileList;
    afx_msg void OnSelchangeComboFilestack();
};
