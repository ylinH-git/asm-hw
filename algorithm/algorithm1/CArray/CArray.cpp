#if 0
#include "CArray.h"
#include <stdexcept>

CArray::CArray()
{
    m_pBuf = new int[1]; //Ĭ����һ��Ԫ�ص�����
    m_nBufSize = 1;
    m_nSize = 0;
}

CArray::CArray(const CArray& ary)
{
}

CArray::CArray(CArray&& ary)
{
}

CArray& CArray::operator=(const CArray& ary)
{
    return *this;
}

CArray::~CArray()
{
}

CArray& CArray::Insert(int val, size_t nIdx)
{
    //������
    if (nIdx > m_nSize)
    {
        throw std::out_of_range("����Խ��");
    }

    //����ڴ��Ƿ��㹻
    if (m_nSize >= m_nBufSize)
    {
        //�����������������µ��ڴ�
        size_t nNewBufSize = static_cast<size_t>(m_nBufSize * 2);
        int* pNewBuf = new int[nNewBufSize];

        //ԭ�������ݿ�������
        memcpy(pNewBuf, m_pBuf, sizeof(int) * m_nSize);

        //�ͷ�ԭ���ڴ�
        delete[]m_pBuf;

        //�����µĻ�����
        m_pBuf = pNewBuf;
        m_nBufSize = nNewBufSize;
    }

    //�ƶ�Ԫ��λ�ã��ڳ��ռ����ڴ洢�µ�����
    for (size_t i = m_nSize; i > nIdx; i--)
    {
        m_pBuf[i] = m_pBuf[i - 1];
    }

    //������������
    m_pBuf[nIdx] = val;
    ++m_nSize;

    return *this;
}

CArray& CArray::InsertHead(int val)
{
    return Insert(val, 0);
}

CArray& CArray::InsertTail(int val)
{
    return Insert(val, m_nSize);
}

CArray& CArray::Delete(int val, size_t nIdx)
{
    return *this;
}

CArray& CArray::DeleteHead(int val)
{
    return *this;
}

CArray& CArray::DeleteTail(int val)
{
    return *this;
}

int& CArray::operator[](size_t nIdx)
{
    return m_pBuf[nIdx];
}

int CArray::Find(int val) const
{
    return 0;
}

bool CArray::IsEmpty() const
{
    return false;
}

void CArray::Clear()
{
}

#endif // 0
