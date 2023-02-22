#pragma once

class CMyString
{
public:
	CMyString(const char* sz = nullptr);
	CMyString(const CMyString& obj);
	~CMyString();

	//��ȡ����
	size_t Len();
	char& At(size_t nIdx);

	//����
	void Copy(const char* sz);
	void Copy(const CMyString& str);

	//ƴ��
	CMyString& Append(const CMyString& str);
	CMyString& Append(const char* sz);
	CMyString& Append(int n);
	CMyString& Append(float f);
	CMyString& Append(double dbl);
	CMyString& Append(short s);

	//����
	int Find(const CMyString& str);
	int Find(const char* sz);
	int ReverseFind(const CMyString& str);
	int ReverseFind(const char* sz);

	//�ִ���ȡ
	CMyString Left(size_t nLen);
	CMyString Right(size_t nLen);
	CMyString Mid(size_t nIdx, size_t nLen);

	//��ʽ��
	CMyString Format(const char* szFmt, ...);

	//ɾ��
	CMyString Delete(size_t nIdx, size_t nLen);

private:
	void Init(const char* sz);
	void Release();

private:
	char* m_sz;       //�ַ���������
	size_t  m_nBufSize; //�ڴ��ܴ�С 
};

