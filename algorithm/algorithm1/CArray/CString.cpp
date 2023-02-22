#include <string>
#include <cstring>
#include <stdarg.h>
#include <assert.h>
#include "CString.h"
using namespace std;

CString::CString(const char* sz):m_sz(nullptr), m_pnRef(nullptr), m_nBufSize(0)
{
	Init(sz);
}

CString::CString(CString&& obj) noexcept : m_sz(obj.m_sz), m_pnRef(obj.m_pnRef), m_nBufSize(obj.m_nBufSize)
{
	obj.m_sz = nullptr;
	obj.m_pnRef = nullptr;
	m_nBufSize = 0;
}

CString::CString(const CString& obj): m_sz(nullptr), m_pnRef(nullptr), m_nBufSize(0)
{
	m_sz = obj.m_sz;
	m_nBufSize = obj.m_nBufSize;
	m_pnRef = obj.m_pnRef;
	if (m_pnRef != nullptr)
	{
		//计数值加1
		*m_pnRef += 1;
	}
}



CString::~CString()
{
	Release();
}

//获取长度
size_t CString::Len() {
	if (m_sz == nullptr)
	{
		return -1;
	}
	return strlen(m_sz);
}

char CString::At(size_t nIdx) {
	assert(m_sz != nullptr && nIdx >= 0 && nIdx < m_nBufSize);
	return m_sz[nIdx];
}

void CString::SetAt(char c, size_t nIdx) {
	Set(c, nIdx);
}

//拷贝
void CString::Copy(const char* sz) {
	if (m_pnRef == nullptr || *m_pnRef != 1)
	{
		Set(sz);
	}
	else
	{
		if (sz == nullptr)
		{
			m_sz = nullptr;
			m_nBufSize = 0;
			return;
		}
		size_t sLen = strlen(sz) + 1;
		if (m_nBufSize < sLen)
		{
			m_nBufSize = sLen;
			m_sz = new char[m_nBufSize];
			if (m_sz == nullptr)
			{
				m_nBufSize = 0;
				return;
			}
		}
		m_nBufSize = sLen;
		strcpy_s(m_sz, m_nBufSize, sz);
	}
}
void CString::Copy(const CString& str) {
	Copy(str.m_sz);
}

//拼接
CString& CString::Append(const CString& str) {
	return Append(str.m_sz);
}
CString& CString::Append(const char* sz) {
	if (sz == nullptr)
	{
		return *this;
	}
	if (m_sz == nullptr)
	{
		Copy(sz);
		return *this;
	}
	size_t newLen = strlen(m_sz) + strlen(sz);
	char* new_sz = nullptr;
	if (m_nBufSize > newLen) {
		newLen = m_nBufSize - 1;
	}
	new_sz = new char[newLen + 1];
	if (new_sz == nullptr)
	{
		m_nBufSize = 0;
		return *this;
	}
	strcpy_s(new_sz, newLen + 1, m_sz);
	strcat_s(new_sz, newLen + 1, sz);
	Set(new_sz);
	return *this;
}
CString& CString::Append(int n) {
	return Append(to_string(n).c_str());
}
CString& CString::Append(float f) {
	return Append(to_string(f).c_str());
}
CString& CString::Append(double dbl) {
	return Append(to_string(dbl).c_str());
}
CString& CString::Append(short s) {
	return Append(to_string(s).c_str());
}

//查找
int CString::Find(const CString& str)
{
	return Find(str.m_sz);
}
int CString::Find(const char* sz)
{
	int i, nFlag = 0;
	if (sz == nullptr)
	{
		return -1;
	}
	size_t szLen = strlen(sz);
	for (i = 0; i < m_nBufSize - szLen; i++)
	{
		if (sz[0] == m_sz[i])
		{
			for (int j = 1; j < szLen - 1; j++)
			{
				if (sz[j] != m_sz[i + j])
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag != 1) {
				return i;
			}
			else {
				nFlag = 0;
			}
		}
	}
	if (i == m_nBufSize - szLen) {
		return -1;
	}
}
int CString::ReverseFind(const CString& str)
{
	return ReverseFind(str.m_sz);
}
int CString::ReverseFind(const char* sz)
{
	int i, nFlag = 0;
	if (sz == nullptr)
	{
		return -1;
	}
	size_t szLen = strlen(sz);
	for (i = m_nBufSize - szLen - 1; i > 0; i--)
	{
		if (sz[0] == m_sz[i])
		{
			for (int j = 1; j < szLen - 1; j++)
			{
				if (sz[j] != m_sz[i + j])
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag != 1) {
				return i;
			}
			else {
				nFlag = 0;
			}
		}
	}

	if (i < 0) {
		return -1;
	}
}

//字串提取
CString CString::Left(size_t nLen) {
	return Mid(0, nLen);
}
CString CString::Right(size_t nLen) {
	return Mid(Len() - nLen, nLen);
}
CString CString::Mid(size_t nIdx, size_t nLen)
{
	if (nIdx >= m_nBufSize && nLen >= m_nBufSize - nIdx)
	{
		return *this;
	}
	char* subStr = new char[nLen + 1];
	size_t nAttr = nIdx;
	strncpy_s(subStr, nLen + 1, m_sz + nAttr, nLen);
	CString sStr(subStr);
	delete[] subStr;
	return sStr;
}

//格式化
CString& CString::Format(const char* szFmt, ...)
{
	if (szFmt == nullptr) {
		return *this;
	}
	va_list arglist;
	va_start(arglist, szFmt);
	size_t nLen = _vscprintf(szFmt, arglist);
	char* szFormat = new char[nLen + 1];
	if (szFormat == nullptr)
	{
		return *this;
	}
	vsprintf_s(szFormat, nLen + 1, szFmt, arglist);
	va_end(arglist);
	Set(szFormat);
	delete[]szFormat;

	return *this;
}


CString& CString::Delete(size_t nIdx, size_t nLen)
{
	if (nIdx >= m_nBufSize && nLen >= m_nBufSize - nIdx)
	{
		return *this;
	}
	if (m_sz == nullptr)
	{
		return *this;
	}
	CString leftStr = Left(nIdx);
	CString rightStr = Right(m_nBufSize - nIdx - nLen - 1);

	size_t newLen = strlen(leftStr.m_sz) + strlen(rightStr.m_sz);
	char* newStr = new char[newLen + 1];
	if (newStr == nullptr)
	{
		return *this;
	}
	strcpy_s(newStr, newLen + 1, leftStr.m_sz);
	strcat_s(newStr, newLen + 1, rightStr.m_sz);
	Set(newStr);
	return *this;
}

void CString::Init(const char* sz)
{
	if (sz == nullptr)
	{
		return;
	}
	m_nBufSize = strlen(sz) + 1;
	m_sz = new char[m_nBufSize];
	m_pnRef = new int(1);
	if (m_sz == nullptr)
	{
		m_nBufSize = 0;
		return;
	}
	strcpy_s(m_sz, m_nBufSize, sz);
}

void CString::Release()
{
	if (m_pnRef == nullptr)
	{
		if (m_sz != nullptr)
		{
			delete[] m_sz;
			m_nBufSize = 0;
		}
	}
	else if (*m_pnRef == 1)
	{
		if (m_sz != nullptr)
		{
			delete[] m_sz;
			m_nBufSize = 0;
		}
		delete m_pnRef;
	}
	else if(*m_pnRef != 0)
	{
		*m_pnRef -= 1;
	}
}



char* CString::ToString() const
{
	return m_sz;
}
CString CString::operator+(const char* sz)
{
	return  CString(m_sz).Append(sz);
}
CString& CString::operator+=(const char* sz)
{
	return Append(sz);
}
CString& CString::operator=(const char* sz)
{
	Copy(sz);
	return *this;
}
CString& CString::operator=(const CString& sz)
{
	Copy(sz.m_sz);
	return *this;
}
CString& CString::operator=(CString&& sz) noexcept
{
	m_sz = sz.m_sz;
	m_pnRef = sz.m_pnRef;
	m_nBufSize = sz.m_nBufSize;
	sz.m_sz = nullptr;
	sz.m_pnRef = nullptr;
	sz.m_nBufSize = 0;
	return *this;
}
char& CString::operator[](size_t idx)
{
	return m_sz[idx];
}
bool CString::operator==(const CString& sz)
{
	return strcmp(m_sz, sz.m_sz) == 0;
}
bool CString::operator<(CString& sz)
{
	return strcmp(m_sz, sz.m_sz) < 0;
}
bool CString::operator>(CString& sz)
{
	return strcmp(m_sz, sz.m_sz) > 0;
}
CString::operator const char* ()
{
	return m_sz;
}
CString CString::ValueOf(bool b)
{
	return StringFromFormat("%s", b ? "true" : "false");
};
 CString CString::ValueOf(char c)
 {
	 return StringFromFormat("%c", c);
 };
 CString CString::ValueOf(double d)
 {
	 return StringFromFormat("%f", d);
 };
 CString CString::ValueOf(float f)
 {
	 return StringFromFormat("%f", f);
 };
 CString CString::ValueOf(int i)
 {
	 return StringFromFormat("%d", i);
 };
 CString CString::ValueOf(long l)
 {
	 return StringFromFormat("%ld", l);
 };
 CString CString::ValueOf(short s)
 {
	 return StringFromFormat("%hd", s);
 };
 CString CString::ValueOf(const char* pChar)
 {
	 return CString(pChar);
 };
 CString CString::CopyValueOf(const char* data, int offset, int count) 
 {
	 CString copyStr(data);
	 return copyStr.Mid(offset, count);
 }
 CString CString::StringFromFormat(const char* fmt, ...)
 {
	 va_list arglist;
	 va_start(arglist, fmt);
	 size_t nLen = _vscprintf(fmt, arglist);
	 char* szFormat = new char[nLen + 1];
	 if (szFormat == nullptr)
	 {
		 return  CString(nullptr);
	 }
	 vsprintf_s(szFormat, nLen + 1, fmt, arglist);
	 va_end(arglist);
	 CString format(szFormat);
	 delete[]szFormat;
	 return format;
 }




 void CString::Set(const char* sz)
 {
	 if (m_pnRef == nullptr)
	 {
		 Init(sz);
	 }
	 else if (*m_pnRef == 1) {
		 Copy(sz);
	 }
	 else
	 {
		 *m_pnRef -= 1;
		 Init(sz);
	 }
 }

 void CString::Set(char c, size_t nIdx)
 {
	 if (m_pnRef == nullptr)
	 {
		 Init(CString::ValueOf(c).ToString());
	 }
	 else if (*m_pnRef == 1) {
		 if (m_sz != nullptr && nIdx >= 0 && nIdx < m_nBufSize)
		 {
			 m_sz[nIdx] = c;
		 }
	 }
	 else
	 {
		 *m_pnRef -= 1;
		 if (m_sz != nullptr && nIdx >= 0 && nIdx < m_nBufSize)
		 {
			 char* newm_sz = new char[m_nBufSize];
			 if (newm_sz != nullptr)
			 {
				 strcpy_s(newm_sz, m_nBufSize, m_sz);
				 newm_sz[nIdx] = c;
				 Init(newm_sz);
			 }
		 }
		
	 }
 }

 CString operator+(const char* sz, CString& str)
 {
	 return CString(sz).Append(str);
 }
