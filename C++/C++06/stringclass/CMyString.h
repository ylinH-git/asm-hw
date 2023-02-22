#pragma once

class CMyString
{
public:
	CMyString(const char* sz = nullptr);
	CMyString(const CMyString& obj);
	~CMyString();

	//获取长度
	size_t Len();
	char& At(size_t nIdx);

	//拷贝
	void Copy(const char* sz);
	void Copy(const CMyString& str);

	//拼接
	CMyString& Append(const CMyString& str);
	CMyString& Append(const char* sz);
	CMyString& Append(int n);
	CMyString& Append(float f);
	CMyString& Append(double dbl);
	CMyString& Append(short s);

	//查找
	int Find(const CMyString& str);
	int Find(const char* sz);
	int ReverseFind(const CMyString& str);
	int ReverseFind(const char* sz);

	//字串提取
	CMyString Left(size_t nLen);
	CMyString Right(size_t nLen);
	CMyString Mid(size_t nIdx, size_t nLen);

	//格式化
	CMyString Format(const char* szFmt, ...);

	//删除
	CMyString Delete(size_t nIdx, size_t nLen);

private:
	void Init(const char* sz);
	void Release();

private:
	char* m_sz;       //字符串缓冲区
	size_t  m_nBufSize; //内存总大小 
};

