#pragma once


// CPenSetting 对话框
struct tagPenAttr
{
	int m_style;
	int m_nWidth;
	COLORREF m_color;
};
class CPenSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CPenSetting)

public:
	CPenSetting(CWnd* pParent = nullptr);   // 标准构造函数
	tagPenAttr GetPenAttr();
	void SetPenAttr(tagPenAttr* attr);
	virtual ~CPenSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	tagPenAttr m_penAttr = {
		PS_SOLID,
		1,
		RGB(0, 0, 0)
	};
public:
	CMFCColorButton m_colorCtrl;
	int m_nWidth;
	CComboBox m_styleCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
