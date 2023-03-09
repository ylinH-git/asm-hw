#pragma once
#include "afxdialogex.h"


// CTlsDlg 对话框

class CTlsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTlsDlg)

public:
	CTlsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTlsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_tlsInfo;
	CListCtrl m_tlsValList;
	void InitTlsInfoList();
	void RenderTlsInfoData();
	void RefreshTlsInfoStyle();
	void InitTlsValList();
	void RenderTlsValListData();
	void RefreshTlsValListStyle();
};
