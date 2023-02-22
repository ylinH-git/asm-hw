#pragma once
// const CMyString& str;
// str.At(0);

class CString
{
public:
	CString(const char* sz = nullptr);
	CString(CString&& obj) noexcept;
	CString(const CString& obj);
	~CString();

	//��ȡ����
	size_t Len();
	char At(size_t nIdx);
	void SetAt(char c, size_t nIdx);

	//����
	void Copy(const char* sz);
	void Copy(const CString& str);

	//ƴ��
	CString& Append(const CString& str);
	CString& Append(const char* sz);
	CString& Append(int n);
	CString& Append(float f);
	CString& Append(double dbl);
	CString& Append(short s);

	//����
	int Find(const CString& str);
	int Find(const char* sz);
	int ReverseFind(const CString& str);
	int ReverseFind(const char* sz);

	//�ִ���ȡ
	CString Left(size_t nLen);
	CString Right(size_t nLen);
	CString Mid(size_t nIdx, size_t nLen);

	//��ʽ��
	CString& Format(const char* szFmt, ...);

	//ɾ��
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

	CString operator+(const char* sz);
	CString& operator+=(const char* sz);
	CString& operator=(const char* sz);
	CString& operator=(const CString& sz);
	CString& operator=(CString&& sz) noexcept;
	char& operator[](size_t idx);
	bool operator==(const CString& sz);
	bool operator<(CString& sz);
	bool operator>(CString& sz);
	explicit operator const char* ();
	friend CString operator+(const char* sz, CString& str);
private:
	void Init(const char* sz);
	void Release();
	void Set(const char* sz);
	void Set(char c, size_t at);

private:
	char* m_sz;       //�ַ���������
	size_t  m_nBufSize; //�ڴ��ܴ�С
	int* m_pnRef;
};

