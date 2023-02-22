#pragma once
// const CMyString& str;
// str.At(0);

class CString
{
public:
	CString(const char* sz = nullptr);
	CString(const CString& obj);
	~CString();

	//获取长度
	size_t Len();
	char& At(size_t nIdx);

	//拷贝
	void Copy(const char* sz);
	void Copy(const CString& str);

	//拼接
	CString& Append(const CString& str);
	CString& Append(const char* sz);
	CString& Append(int n);
	CString& Append(float f);
	CString& Append(double dbl);
	CString& Append(short s);

	//查找
	int Find(const CString& str);
	int Find(const char* sz);
	int ReverseFind(const CString& str);
	int ReverseFind(const char* sz);

	//字串提取
	CString Left(size_t nLen);
	CString Right(size_t nLen);
	CString Mid(size_t nIdx, size_t nLen);

	//格式化
	CString& Format(const char* szFmt, ...);

	//删除
	CString& Delete(size_t nIdx, size_t nLen);
	CString& Delete(const char* sz);
	CString& Delete(const CString& str);

	// true ==> "true"
	static CString ValueOf(bool); 
	// a ==> "a"
	static CString ValueOf(char c);  
	// 3.5 ==> "3.5"
	static CString ValueOf(double d); 
	// 4.5f ==> "4.5"
	static CString ValueOf(float f);  
	// 123 ==> "123"
	static CString ValueOf(int i);     
	// 123 ==> "123"
	static CString ValueOf(long i);
	// 123 ==> "123"
	static CString ValueOf(short i);  
	// char[] = "123" ==> string "123"
	static CString ValueOf(const char* data); 
	// char[] = "123" ==> string "123"
	static CString CopyValueOf(const char* data, int offset, int count); 
	// ("%d", 123) ==> string "123"
	static CString StringFromFormat(const char* fmt, ...); 
	char* ToString() const;

private:
	void Init(const char* sz);
	void Release();

private:
	char* m_sz;       //字符串缓冲区
	size_t  m_nBufSize; //内存总大小
	static int class_num;
	const int id; // 字符串id
};

