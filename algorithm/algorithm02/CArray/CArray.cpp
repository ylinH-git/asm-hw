#if 0
#include "CArray.h"
#include <stdexcept>

CArray::CArray()
{
    m_pBuf = new int[1]; //默认有一个元素的容量
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
    //检查参数
    if (nIdx > m_nSize)
    {
        throw std::out_of_range("索引越界");
    }

    //检查内存是否足够
    if (m_nSize >= m_nBufSize)
    {
        //缓冲区不够，申请新的内存
        size_t nNewBufSize = static_cast<size_t>(m_nBufSize * 2);
        int* pNewBuf = new int[nNewBufSize];

        //原来的数据拷贝过来
        memcpy(pNewBuf, m_pBuf, sizeof(int) * m_nSize);

        //释放原来内存
        delete[]m_pBuf;

        //保存新的缓冲区
        m_pBuf = pNewBuf;
        m_nBufSize = nNewBufSize;
    }

    //移动元素位置，腾出空间用于存储新的数据
    for (size_t i = m_nSize; i > nIdx; i--)
    {
        m_pBuf[i] = m_pBuf[i - 1];
    }

    //保存插入的数据
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
