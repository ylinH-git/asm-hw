#include "CList.h"
#include <stdexcept>      // std::out_of_range
CList::CList()
{
	m_pHeadGuard = new Node();
	m_pTailGuard = new Node();

	m_pHeadGuard->m_next = m_pTailGuard;
	m_pTailGuard->m_prev = m_pHeadGuard;
}

CList::CList(const CList& lst)
{
	m_pHeadGuard = new Node();
	m_pTailGuard = new Node();

	m_pHeadGuard->m_next = m_pTailGuard;
	m_pTailGuard->m_prev = m_pHeadGuard;
	CopyList(lst);
}

CList::CList(CList&& lst) noexcept
{
	m_pHeadGuard = lst.m_pHeadGuard;
	m_pTailGuard = lst.m_pTailGuard;
	m_nSize = lst.m_nSize;
	lst.m_pHeadGuard = nullptr;
	lst.m_pTailGuard = nullptr;
	lst.m_nSize = 0;
}

CList::~CList()
{
	Clear();
	delete m_pHeadGuard;
	delete m_pTailGuard;
	m_pHeadGuard = nullptr;
	m_pTailGuard = nullptr;
}

CList& CList::operator=(const CList& lst)
{
	CopyList(lst);
	return *this;
}

void CList::CopyList(const CList& lst) 
{
	for (int i = 0; i < lst.m_nSize; i++)
	{
		InsertTail(const_cast<CList&>(lst)[i]);
	}
}

CList& CList::InsertHead(int val)
{
	return Insert(val, 0);
}

CList& CList::InsertTail(int val)
{
	return Insert(val, m_nSize);
}

CList& CList::Insert(int val, size_t nIdx)
{
	if (nIdx < 0 || nIdx > m_nSize)
	{
		throw std::out_of_range("out of range");
	}
	Node* pCurrentNode = IdxToNode(nIdx);
	Node* pPreNode = pCurrentNode->m_prev;
	Node* pNewNode = new Node(val);
	pPreNode->m_next = pNewNode;
	pCurrentNode->m_prev = pNewNode;


	pNewNode->m_prev = pPreNode;
	pNewNode->m_next = pCurrentNode;
	++m_nSize;
	return *this;
}

CList& CList::DeleteHead()
{
	return Delete(0);
}

CList& CList::DeleteTail()
{
	return Delete(m_nSize - 1);
}

CList& CList::Delete(size_t nIdx)
{
	if (nIdx < 0 || nIdx >= m_nSize)
	{
		throw std::out_of_range("out of range");
	}
	Node* pDelNode = IdxToNode(nIdx);
	Node* pPreNode = pDelNode->m_prev;
	Node* pNextNode = pDelNode->m_next;
	pPreNode->m_next = pNextNode;
	pNextNode->m_prev = pPreNode;
	delete pDelNode;
	--m_nSize;
	return *this;
}

int& CList::operator[](size_t nIdx)
{
	if (nIdx < 0 || nIdx >= m_nSize)
	{
		throw std::out_of_range("out of range");
	}
	Node* pNode = IdxToNode(nIdx);
	return pNode->m_val;
}

void CList::Clear()
{
	while (!IsEmpty())
	{
		DeleteHead();
	}
}

size_t CList::Find(int val) const
{
	Node* pNextNode = m_pHeadGuard;
	for (int i = 0; i < m_nSize; i++)
	{
		pNextNode = m_pHeadGuard->m_next;
		if (pNextNode->m_val == val)
		{
			return i;
		}
	}
	return -1;
}

size_t CList::Size() const
{
	return m_nSize;
}

bool CList::IsEmpty() const
{
	return m_nSize == 0;
}

Node* CList::IdxToNode(size_t nIdx)
{
	Node* pNextNode = m_pHeadGuard->m_next;
	for (int i = 0; i < m_nSize; i++)
	{
		if (i == nIdx) 
		{
			break;
		}
		pNextNode = pNextNode->m_next;
	}
	return pNextNode;
}
