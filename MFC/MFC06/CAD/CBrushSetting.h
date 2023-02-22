#pragma once


// CBrushSetting 对话框

class CBrushSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CBrushSetting)

public:
	CBrushSetting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBrushSetting();
	LOGBRUSH GetBrushAttr();
	void SetBrushAttr(LOGBRUSH* attr);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_BRUSH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	LOGBRUSH  m_brushAttr = {
		BS_SOLID,
		RGB(255, 255, 255),
		HS_BDIAGONAL
	};
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CComboBox m_styleCtrl;
	CComboBox m_hatchStyle;
	CMFCColorButton m_color;
};
