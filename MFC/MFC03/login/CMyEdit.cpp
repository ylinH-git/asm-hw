#include "CMyEdit.h"
BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (('a' <= nChar) && (nChar <= 'z') ||
		('A' <= nChar) && (nChar <= 'Z') ||
		('0' <= nChar) && (nChar <= '9') ||
		nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
	{
		MessageBeep(-1);
	}
}
