#pragma once


// CClassDlg 对话框

class CClassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClassDlg)

public:
	CClassDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClassDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CLASS_ATTR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_className;
	CString m_classStyle;
	CComboBox m_classStyleBox;
	CString m_classByte;
	CComboBox m_classByteBox;
	CString m_insHandle;
	CString m_winProcess;
	CString m_menuName;
	CString m_iconHandle;
	CString m_cursorHandle;
	CString m_classAtom;
	CString m_bgPen;
	CString m_winByte;
};
