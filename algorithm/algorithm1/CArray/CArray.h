#pragma once
#include <stdexcept>
#include <utility>
template<typename T>
class CArray
{
public:
  CArray(){
      Init();
  };
  CArray(const CArray& ary) {
      Init();
  };
  CArray(CArray&& ary) noexcept {};
  CArray& operator=(const CArray& ary) {};
  CArray& operator=(CArray&& ary) = delete;
  virtual ~CArray() {};

  CArray& Insert(const T& val, size_t nIdx) 
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
        T* pNewBuf = new T[nNewBufSize];

        //原来的数据拷贝过来
        for (int i = 0; i < m_nBufSize; i++) {
            pNewBuf[i] = std::move(m_pBuf[i]);
        }

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
    m_pBuf[nIdx] = std::move(val);
    ++m_nSize;

    return *this;
  } //指定位置插入
  CArray& InsertHead(const T& val)
  {
      return Insert(val, 0);
  };          //头部位置插入
  CArray& InsertTail(const T& val)
  {
      return Insert(val, m_nSize);
  };          //尾部位置插入

  CArray& Delete(size_t nIdx) 
  {
      //检查参数
      if (nIdx > m_nSize || nIdx < 0)
      {
          throw std::out_of_range("索引越界");
      }

      //检查内存是否足够
      if (m_nSize <= m_nBufSize / 2)
      {
          //缓冲区不够，申请新的内存
          size_t nNewBufSize = static_cast<size_t>(m_nBufSize / 2);
          T* pNewBuf = new T[nNewBufSize];

          //原来的数据拷贝过来
          for (int i = 0; i < m_nSize; i++) {
              pNewBuf[i] = std::move(m_pBuf[i]);
          }

          //释放原来内存
          delete[]m_pBuf;

          //保存新的缓冲区
          m_pBuf = pNewBuf;
          m_nBufSize = nNewBufSize;
      }

      //移动元素位置，腾出空间用于存储新的数据
      for (int i = nIdx; i < m_nSize - 1; i++)
      {
          m_pBuf[i] = std::move(m_pBuf[i + 1]);
      }
      --m_nSize;
      return *this;
  }; //指定位置删除
  CArray& DeleteHead() 
  {
      Delete(0);
      return *this;
  };          //头部位置删除
  CArray& DeleteTail()
  {
      Delete(m_nSize);
      return *this;
  };          //尾部位置删除

  T& operator[](size_t nIdx)
  {
      if (nIdx < 0 || nIdx >= m_nSize)
      {
          throw std::out_of_range("索引越界");
      } 
      return m_pBuf[nIdx];
  };         //修改 + 访问

  int  Find(const T& val)const
  {
      for (int i = 0; i < m_nSize; i++)
      {
          if(m_pBuf[i] == val)
          {
              return i;
          }
      }
      return -1;
  };               //查找

  bool IsEmpty()const 
  {
      return m_nSize == 0;
  };                    //判空
  void Clear()
  {
      //释放原来内存
      delete[]m_pBuf;
      Init();
  };                           //清空


private:
  T*   m_pBuf     = nullptr; //元素缓冲区
  size_t m_nSize    = 0;       //元素个数    内容content
  size_t m_nBufSize = 0;       //缓冲区大小  容量capacity
  void Init() 
  {
      m_pBuf = new T[1]; //默认有一个元素的容量
      m_nBufSize = 1;
      m_nSize = 0;
  }
};
