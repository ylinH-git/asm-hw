#pragma once
#include <afxwin.h>
#include <vector>
class CDlgApp :
    public CWinApp
{
public:
    virtual BOOL InitInstance();
	enum class eGender {
		MALE,
		FEMALE,
		UNKNOW,
	};
	typedef struct tagMember {
		int m_id;
		CString m_name;
		CString m_tel;
		eGender m_gender;
	} MEMBER;
	std::vector<tagMember> g_members;
};
extern CDlgApp theApp;
