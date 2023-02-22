#include <string>
#include <cstring>
#include <stdarg.h>
#include <assert.h>
#include "CMyString.h"
using namespace std;
CMyString::CMyString(const char* sz)
{
	Init(sz);
}

CMyString::CMyString(const CMyString& obj)
{
	Init(obj.m_sz);
}

CMyString::~CMyString()
{
	Release();
}

//获取长度
size_t CMyString::Len() {
	if (m_sz == nullptr)
	{
		return -1;
	}
	return strlen(m_sz);
}

char& CMyString::At(size_t nIdx) {
	assert(m_sz != nullptr && nIdx >= 0 && nIdx < m_nBufSize);
	return m_sz[nIdx];
}

//拷贝
void CMyString::Copy(const char* sz) {
	if(sz == nullptr)
	{ 
		m_sz = nullptr;
		m_nBufSize = 0;
		return;
	}
	size_t sLen = strlen(sz) + 1;
	if (m_nBufSize < sLen)
	{
		Release();
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
void CMyString::Copy(const CMyString& str) {
	Copy(str.m_sz);
}

//拼接
CMyString& CMyString::Append(const CMyString& str) {
	return Append(str.m_sz);
}
CMyString& CMyString::Append(const char* sz) {
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
	if (m_nBufSize > newLen) {
		strcat_s(m_sz, m_nBufSize, sz);
		m_nBufSize = newLen + 1;
	}
	else
	{
		char* new_sz = new char[newLen + 1];
		if (new_sz == nullptr)
		{
			m_nBufSize = 0;
			return *this;
		}
		strcpy_s(new_sz, newLen + 1, m_sz);
		strcat_s(new_sz, newLen + 1, sz);
		Release();
		m_sz = new_sz;
	}
	m_nBufSize = newLen + 1;
	return *this;
}
CMyString& CMyString::Append(int n) {
	return Append(to_string(n).c_str());
}
CMyString& CMyString::Append(float f) {
	return Append(to_string(f).c_str());
}
CMyString& CMyString::Append(double dbl) {
	return Append(to_string(dbl).c_str());
}
CMyString& CMyString::Append(short s) {
	return Append(to_string(s).c_str());
}

//查找
int CMyString::Find(const CMyString& str) 
{ 
	return Find(str.m_sz);
}
int CMyString::Find(const char* sz) 
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
int CMyString::ReverseFind(const CMyString& str) 
{ 
	return ReverseFind(str.m_sz); 
}
int CMyString::ReverseFind(const char* sz) 
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
CMyString CMyString::Left(size_t nLen) {
	if (nLen >= m_nBufSize)
	{
		return *this;
	}
	char* subStr = new char[nLen + 1];
	strncpy_s(subStr, nLen + 1, m_sz, nLen);
	CMyString sStr(subStr);
	delete[] subStr;
	return sStr;
}
CMyString CMyString::Right(size_t nLen) {
	if (nLen >= m_nBufSize)
	{
		return *this;
	}
	char* subStr = new char[nLen + 1];
	size_t nAttr = m_nBufSize - nLen - 1;
	strncpy_s(subStr, nLen + 1, m_sz + nAttr, nLen);
	CMyString sStr(subStr);
	delete[] subStr;
	return sStr;
}
CMyString CMyString::Mid(size_t nIdx, size_t nLen)
{
	if (nIdx >= m_nBufSize && nLen >= m_nBufSize - nIdx)
	{
		return *this;
	}
	char* subStr = new char[nLen + 1];
	size_t nAttr = nIdx;
	strncpy_s(subStr, nLen + 1, m_sz + nAttr, nLen);
	CMyString sStr(subStr);
	delete[] subStr;
	return sStr;
}

//格式化
CMyString CMyString::Format(const char* szFmt, ...)
{
	if (szFmt == nullptr) {
		return CMyString();
	}
	va_list arglist;
	va_start(arglist, szFmt);
	size_t nLen = _vscprintf(szFmt, arglist);
	char* cFormat = new char[nLen + 1];
	if (cFormat == nullptr)
	{
		return CMyString();
	}
	vsprintf_s(cFormat, nLen + 1, szFmt, arglist);
	va_end(arglist);
	return CMyString(cFormat);
}


CMyString CMyString::Delete(size_t nIdx, size_t nLen)
{
	if (nIdx >= m_nBufSize && nLen >= m_nBufSize - nIdx)
	{
		return *this;
	}
	if (m_sz == nullptr)
	{
		return *this;
	}
	CMyString leftStr = Left(nIdx);
	CMyString rightStr = Right(m_nBufSize - nIdx - nLen - 1);

	size_t newLen = strlen(leftStr.m_sz) + strlen(rightStr.m_sz);
	char* newStr = new char[newLen + 1];
	if (newStr == nullptr)
	{
		return *this;
	}
	Release();
	strcpy_s(newStr, newLen + 1, leftStr.m_sz);
	strcat_s(newStr, newLen + 1, rightStr.m_sz);
	m_sz = newStr;
	m_nBufSize = newLen + 1;
	return *this;
}

void CMyString::Init(const char* sz)
{
	if (sz == nullptr)
	{
		return;
	}
	m_nBufSize = strlen(sz) + 1;
	m_sz = new char[m_nBufSize];
	if (m_sz == nullptr)
	{
		m_nBufSize = 0;
		return;
	}
	strcpy_s(m_sz, m_nBufSize, sz);
}

void CMyString::Release()
{
	if (m_sz != nullptr)
	{
		delete[] m_sz;
		m_nBufSize = 0;
	}
}

