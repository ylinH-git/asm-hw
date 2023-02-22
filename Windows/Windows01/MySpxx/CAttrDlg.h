#pragma once


// CAttrDlg 对话框
#include "CClassDlg.h"
#include "CGeneralDlg.h"
#include "CStyleDlg.h"
#include "CWinDlg.h"
#include "CProcessDlg.h"

class CAttrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAttrDlg)

public:
	CAttrDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAttrDlg();
	void SetWinBaseAttr(HWND currHwnd,  CString handle, CString title, CString className);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_ATTRBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitProcessDlg();
	void InitClassDlg();
	void SetClassBgPen();
	void SetClassStyle();
	void InitWinDlg();
	void InitStyleDlg();
	void SetExStyleList(const LONG& exStyle);
	void SetWinStyleList(const LONG& winStyle);
	void InitGeneralDlg();
	CTabCtrl m_tab;

private:
	HWND m_currHwnd;
	CString m_handle;
	CString m_title;
	CString m_class;
	CGeneralDlg m_generalDlg;
	CClassDlg m_classDlg;
	CStyleDlg m_styleDlg;
	CWinDlg m_winDlg;
	CProcessDlg m_prcessDlg;
public:
	afx_msg void OnSelchangeTabAttr(NMHDR* pNMHDR, LRESULT* pResult);
};
