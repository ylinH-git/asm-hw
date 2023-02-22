
// RegManagerDlg.h: 头文件
//

#pragma once
#include <map>
#include "CAddKeyDlg.h"
#include "CAddStrDlg.h"
#include "CAddDwordDlg.h"
#include "CAddQwordDlg.h"

// CRegManagerDlg 对话框
class CRegManagerDlg : public CDialogEx
{
// 构造
public:
	CRegManagerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CRegManagerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	void GetCurValues(const HTREEITEM& classRoot);

	void SetValue(const size_t& nRowIdx, char* szName, const DWORD& dwType, const DWORD& dwDataLen, char* pData);

	void InitKeyTree();

	void GetSubKeys(const HTREEITEM& classRoot);

	void InitValueList();

	void SetListColumnSize();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
public:
	CString m_curPath;
	HKEY m_curHkey;
	CTreeCtrl m_keyTree;
	CListCtrl m_valueList;
	std::map<HKEY, PVOID> m_keyList;
//	afx_msg void OnClickTreeKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedTreeKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeKey(NMHDR* pNMHDR, LRESULT* pResult);
	void GenerateChildItems(const HTREEITEM& curItem);
	afx_msg void OnItemexpandingTreeKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedAddkey();
	afx_msg void OnBnClickedDelkey();
//	afx_msg void OnBnClickedAddvalue();
	afx_msg void OnBnClickedDelvalue();
	afx_msg void OnBnClickedAdddword();
	void SetDwordValue(CAddDwordDlg& dlg, const HKEY& curItemData, HTREEITEM& curItem);
	afx_msg void OnBnClickedAddqword();
	void SetQwordValue(CAddQwordDlg& dlg, const HKEY& curItemData, HTREEITEM& curItem);
	afx_msg void OnBnClickedAddstr();
	void SetStrValue(CAddStrDlg& dlg, const HKEY& curItemData, HTREEITEM& curItem);
	afx_msg void OnItemdblclickListValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListValue(NMHDR* pNMHDR, LRESULT* pResult);
};
