#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <windowsx.h>
#include "resource.h"

enum class eGender {
	MALE,
	FEMALE,
	UNKNOW,
};
typedef struct tagMember {
	int m_id;
	std::string m_name;
	std::string m_tel;
	eGender m_gender;
} MEMBER;
extern HINSTANCE g_hIns;
extern std::vector<tagMember> g_members;