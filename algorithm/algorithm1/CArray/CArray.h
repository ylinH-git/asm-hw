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
        T* pNewBuf = new T[nNewBufSize];

        //ԭ�������ݿ�������
        for (int i = 0; i < m_nBufSize; i++) {
            pNewBuf[i] = std::move(m_pBuf[i]);
        }

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
    m_pBuf[nIdx] = std::move(val);
    ++m_nSize;

    return *this;
  } //ָ��λ�ò���
  CArray& InsertHead(const T& val)
  {
      return Insert(val, 0);
  };          //ͷ��λ�ò���
  CArray& InsertTail(const T& val)
  {
      return Insert(val, m_nSize);
  };          //β��λ�ò���

  CArray& Delete(size_t nIdx) 
  {
      //������
      if (nIdx > m_nSize || nIdx < 0)
      {
          throw std::out_of_range("����Խ��");
      }

      //����ڴ��Ƿ��㹻
      if (m_nSize <= m_nBufSize / 2)
      {
          //�����������������µ��ڴ�
          size_t nNewBufSize = static_cast<size_t>(m_nBufSize / 2);
          T* pNewBuf = new T[nNewBufSize];

          //ԭ�������ݿ�������
          for (int i = 0; i < m_nSize; i++) {
              pNewBuf[i] = std::move(m_pBuf[i]);
          }

          //�ͷ�ԭ���ڴ�
          delete[]m_pBuf;

          //�����µĻ�����
          m_pBuf = pNewBuf;
          m_nBufSize = nNewBufSize;
      }

      //�ƶ�Ԫ��λ�ã��ڳ��ռ����ڴ洢�µ�����
      for (int i = nIdx; i < m_nSize - 1; i++)
      {
          m_pBuf[i] = std::move(m_pBuf[i + 1]);
      }
      --m_nSize;
      return *this;
  }; //ָ��λ��ɾ��
  CArray& DeleteHead() 
  {
      Delete(0);
      return *this;
  };          //ͷ��λ��ɾ��
  CArray& DeleteTail()
  {
      Delete(m_nSize);
      return *this;
  };          //β��λ��ɾ��

  T& operator[](size_t nIdx)
  {
      if (nIdx < 0 || nIdx >= m_nSize)
      {
          throw std::out_of_range("����Խ��");
      } 
      return m_pBuf[nIdx];
  };         //�޸� + ����

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
  };               //����

  bool IsEmpty()const 
  {
      return m_nSize == 0;
  };                    //�п�
  void Clear()
  {
      //�ͷ�ԭ���ڴ�
      delete[]m_pBuf;
      Init();
  };                           //���


private:
  T*   m_pBuf     = nullptr; //Ԫ�ػ�����
  size_t m_nSize    = 0;       //Ԫ�ظ���    ����content
  size_t m_nBufSize = 0;       //��������С  ����capacity
  void Init() 
  {
      m_pBuf = new T[1]; //Ĭ����һ��Ԫ�ص�����
      m_nBufSize = 1;
      m_nSize = 0;
  }
};
