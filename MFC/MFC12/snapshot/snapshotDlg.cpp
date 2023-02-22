
// snapshotDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "snapshot.h"
#include "snapshotDlg.h"
#include "afxdialogex.h"
#define CTRL_SHIFT_Z_HOT_KEY 0xC000

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


// CsnapshotDlg 对话框



CsnapshotDlg::CsnapshotDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNAPSHOT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsnapshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsnapshotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HOTKEY()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CsnapshotDlg 消息处理程序

BOOL CsnapshotDlg::OnInitDialog()
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
	RegisterHotKey(GetSafeHwnd(), CTRL_SHIFT_Z_HOT_KEY, MOD_CONTROL | MOD_SHIFT, 'Z');
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsnapshotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsnapshotDlg::OnPaint()
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
HCURSOR CsnapshotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsnapshotDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId == CTRL_SHIFT_Z_HOT_KEY)
	{
		// 获取鼠标所在窗口
		CPoint pt;
		GetCursorPos(&pt);
		// TODO:有些软件的句柄拿不到，比如notion、谷歌浏览器
		CWnd* pWndDst = CWnd::WindowFromPoint(pt);

		// 窗口DC
		CDC* pDc = pWndDst->GetDC();
		CRect rc;
		pWndDst->GetClientRect(&rc);
		int nBitsPixel = pDc->GetDeviceCaps(BITSPIXEL);
		int nBytePixel = ceil(nBitsPixel / 8);

		// 获取窗口页数并从窗口拷贝图片

		int nMaxPage = 1;

		CImage img;

		CImage pageImg;
		pageImg.Create(rc.Width(), rc.Height() + 5, nBitsPixel);

		HDC hPageImgDC = pageImg.GetDC();
		auto pageBufSize = rc.Width() * nBytePixel;
		char* imgBuf = nullptr;
		char* lastBitsBuf = new char[pageBufSize];
		Sleep(500);
		while (true)
		{
			::BitBlt(hPageImgDC, 0, 0, rc.Width(), rc.Height() + 5, pDc->GetSafeHdc(), 0, 0, SRCCOPY);
			char* pBits = (char*)pageImg.GetBits();
			int pPitch = pageImg.GetPitch() + pageBufSize;
			char* curBits = (char*)pageImg.GetBits() + (pageImg.GetPitch() * (pageImg.GetHeight()/ 2));
			if (memcmp(lastBitsBuf, curBits, pageBufSize) == 0) {
				break;
			}
			memcpy_s(lastBitsBuf, pageBufSize, curBits, pageBufSize);
			if (img.IsNull())
			{
				img.Create(rc.Width(), (rc.Height() + 5) * nMaxPage, nBitsPixel);
				HDC imgDC = img.GetDC();
				::BitBlt(imgDC, 0, 0, rc.Width(), rc.Height() + 5, hPageImgDC, 0, 0, SRCCOPY);
				img.ReleaseDC();
			}
			else
			{
				HDC imgDC = img.GetDC();
				CImage tempImg;
				tempImg.Create(rc.Width(), (rc.Height() + 5) * nMaxPage, nBitsPixel);
				HDC tempImgDC = tempImg.GetDC();
				::BitBlt(tempImgDC, 0, 0, rc.Width(), (rc.Height() + 5) * (nMaxPage - 1), imgDC, 0, 0, SRCCOPY);
				::BitBlt(tempImgDC, 0, (rc.Height() + 5) * (nMaxPage - 1), rc.Width(), rc.Height() + 5, hPageImgDC, 0, 0, SRCCOPY);
				img.ReleaseDC();
				img.Destroy();
				img.Create(rc.Width(), (rc.Height() + 5) * nMaxPage, nBitsPixel);
				imgDC = img.GetDC();
				::BitBlt(imgDC, 0, 0, rc.Width(), (rc.Height() + 5) * nMaxPage, tempImgDC, 0, 0, SRCCOPY);
				img.ReleaseDC();
				tempImg.ReleaseDC();
			}
			
			nMaxPage++;
			keybd_event(VK_NEXT, 0, 0, 0);
			keybd_event(VK_NEXT, 0, KEYEVENTF_KEYUP, 0);
			Sleep(500);
		}
		delete[] lastBitsBuf;
		pageImg.ReleaseDC();

		// 保存
		CString fileName = "snapshot_";
		CTime t = CTime::GetCurrentTime();
		CString timeFormat = t.Format("%Y%m%d_%H%M%S");
		img.Save(fileName + timeFormat.GetString() + ".png");

		pWndDst->ReleaseDC(pDc);
		if (imgBuf != nullptr)
		{
			delete[] imgBuf;
		}
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


//void CsnapshotDlg::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	CWnd* pWndDst = CWnd::WindowFromPoint(point);
//
//	// 窗口DC
//	CDC* pDc = pWndDst->GetDC();
//	CRect rc;
//	pWndDst->GetClientRect(&rc);
//	pDc->DrawEdge(CRect(0, 0, rc.Width(), rc.Height()), EDGE_SUNKEN, BF_FLAT | BF_RECT);
//	CDialogEx::OnMouseMove(nFlags, point);
//}
