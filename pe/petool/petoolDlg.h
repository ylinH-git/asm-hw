
// petoolDlg.h: 头文件
//

#pragma once
#include "CFileInfoDlg.h"
#include "CDosHeaderDlg.h"
#include "CNtHeadersDlg.h"
#include "CFileHeaderDlg.h"
#include "COptionalHeaderDlg.h"
#include "CDataDirectoriesDlg.h"
#include "CSectionHeaderDlg.h"
#include "CAddrConvertDlg.h"
#include "CImportDirectoryDlg.h"
#include "CImportAddDlg.h"
#include "CSectionInsertDlg.h"
#include "CExportDirectoryDlg.h"
#include "CRelocationDirectoryDlg.h"

// CpetoolDlg 对话框
class CpetoolDlg : public CDialogEx
{
// 构造
public:
	CpetoolDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CpetoolDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PETOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void CreatePeTree(CString fileName);
	void InitFileInfoDlg();
	void InitDosHeaderDlg();
	void InitNtHeadersDlg();
	void InitFileHeaderDlg();
	void InitOptionalHeaderDlg();
	void InitDataDirectoriesDlg();
	void InitSectionHeaderDlg();
	void InitAddrConvertDlg();
	void InitImportDirectoryDlg();
	void InitImportAddDlg();
	void InitSectionInsertDlg();
	void InitExportDirectoryDlg();
	void InitRelocationDirectoryDlg();
	void GetDosStruct();
	void GetNtStruct();
	void GetFileHStruct();
	void GetOptionalStruct();
	void GetDataDirStruct();
	void GetSectionHeaders();
	void GetImportDirectory();
	void GetExportDirectory();
	void GetRelocationDirectory();
	void HideAllDlg();
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	bool m_bIsInited;
	CTreeCtrl m_peTree;
	CString m_filePath;
	afx_msg void OnSelchangedTreePe(NMHDR* pNMHDR, LRESULT* pResult);
	HTREEITEM m_peFileInfo;
	HTREEITEM m_dosHeader;
	HTREEITEM m_ntHeader;
	HTREEITEM m_fileHeader;
	HTREEITEM m_optionalHeader;
	HTREEITEM m_dataDirectories;
	HTREEITEM m_sectionHeaders;
	HTREEITEM m_importDirectory;
	HTREEITEM m_resDirectory;
	HTREEITEM m_relocationDirectory;
	HTREEITEM m_addrConverter;
	HTREEITEM m_depWalker;
	HTREEITEM m_hexEditor;
	HTREEITEM m_identifier;
	HTREEITEM m_importAdder;
	HTREEITEM m_quickDisasm;
	HTREEITEM m_rebuilder;
	HTREEITEM m_resEditor;
	HTREEITEM m_upx;
	HTREEITEM m_secctionInsert;
	HTREEITEM m_exportDirectory;
	CFileInfoDlg m_fileInfoDlg;
	CDosHeaderDlg m_dosHeaderDlg;
	CNtHeadersDlg m_ntHeaderDlg;
	CFileHeaderDlg m_fileHeaderDlg;
	COptionalHeaderDlg m_optionalHeaderDlg;
	CDataDirectoriesDlg m_dataDirectoriesDlg;
	CSectionHeaderDlg m_sectionHeaderDlg;
	CAddrConvertDlg m_addrConverDlg;
	CImportDirectoryDlg m_importDirectoryDlg;
	CImportAddDlg m_importAddDlg;
	CSectionInsertDlg m_sectionInsertDlg;
	CExportDirectoryDlg m_exportDirectoryDlg;
	CRelocationDirectoryDlg m_relocationDirectoryDlg;
};
