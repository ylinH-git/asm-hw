
// GameHelperDlg.h: 头文件
//

#pragma once


// CGameHelperDlg 对话框
class CGameHelperDlg : public CDialogEx
{
// 构造
public:
	CGameHelperDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEHELPER_DIALOG };
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
	void GetHProc();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hProc = nullptr;
	DWORD m_bulletsNum = 0;
	BYTE* m_bullets = nullptr;
	UINT_PTR m_avoidTimer = NULL;
	HWND m_hwndDst = nullptr;
public:
	afx_msg void OnBnClickedGethproc();
	afx_msg void OnBnClickedUnbeatable();
	void ChangeMemory(const LPVOID& baseAddress, const SIZE_T& changeSize, LPCVOID btCode, SIZE_T& nBytesOfWrited);
	afx_msg void OnBnClickedAvoid();
	void GetPlanePos(CPoint& plane);
	afx_msg void OnBnClickedSpeed();
	afx_msg void OnBnClickedBullet();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GetBullets();
	BOOL NeedMove(const CPoint& plane);
	BOOL NoBullet(int x, int y);
	BOOL isDead();
	void GetBulletsNum();
};
