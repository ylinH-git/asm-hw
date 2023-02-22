#pragma once
struct Node
{
  Node(int val):m_val(val){}
  Node():m_val(){}

  int   m_val;            //����
  Node* m_prev = nullptr; //ǰ�����ĵ�ַ
  Node* m_next = nullptr; //��̽��ĵ�ַ
};

class CList
{
public:
  static const size_t INVALID_IDX = -1;
public:
  CList();
  CList(const CList& lst);
  CList(CList&& lst) noexcept;
  virtual ~CList();
  CList& operator=(const CList& lst);
  CList& operator=(CList&& lst) = delete;

  CList& InsertHead(int val);
  CList& InsertTail(int val);
  CList& Insert(int val, size_t nIdx); 

  CList& DeleteHead();
  CList& DeleteTail();
  CList& Delete(size_t nIdx);           

  int& operator[](size_t nIdx);      

  void   Clear();
  size_t Find(int val)const;            
  size_t Size()const;
  bool   IsEmpty()const;

private:
  Node* IdxToNode(size_t nIdx);
  void CopyList(const CList& lst);

private:
  Node* m_pHeadGuard = nullptr; //ͷ�ڱ�
  Node* m_pTailGuard = nullptr; //β�ڱ�
  size_t m_nSize = 0;           //����
};

