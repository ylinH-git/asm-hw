#pragma once
#include <iostream>
#include "CStack.h"
#include "CQueue.h"
template<typename T>
struct  Node
{
	Node(T val)
	{
		m_val = new T(val);
	}
	~Node() {
		if (m_val != nullptr)
		{
			delete m_val;
		}
	}
	T* m_val;                 //数据
	Node* m_pParent = nullptr; //父节点
	Node* m_pLeft   = nullptr; //左孩子
	Node* m_pRight  = nullptr; //右孩子
	size_t m_nHeight = 1;
};

template<typename T>
class CAVL
{
public:
  //六件套
	CAVL() : m_pRoot(nullptr)
	{}
	CAVL(const CAVL& bstTree) : m_pRoot(nullptr)
	{
		CopyDeep(bstTree);
	}
	CAVL(CAVL&& bstTree) noexcept : m_pRoot(nullptr)
	{
		m_pRoot = bstTree.m_pRoot;
		bstTree.m_pRoot = nullptr;
	}
	CAVL& operator=(const CAVL& bstTree)
	{
		if (&bstTree != this)
		{
			Clear();
			CopyDeep(bstTree);
		}
		return *this;
	}
	CAVL& operator=(CAVL&& bstTree) = delete;
	~CAVL()
	{
		Clear();
	}

	CAVL& Insert(T val)
	{
		//判空
		if (IsEmpty())
		{
			m_pRoot = new Node<T>(val); //新节点作为空树的第一个节点(根节点)
			return *this;
		}

		//从根节点开始查找位置
		Node<T>* pNode = m_pRoot;
		while (pNode != nullptr)
		{
			//判断
			if (val < *(pNode->m_val))
			{
				//如果没有左孩子，则新节点成为为左孩子
				if (pNode->m_pLeft == nullptr)
				{
					Node<T>* pNew = new Node<T>(val);
					pNode->m_pLeft = pNew;
					pNew->m_pParent = pNode;

					UpdateHeight(pNew);
					//插入成功，结束
					break;
				}

				//到左子树查找
				pNode = pNode->m_pLeft;
			}
			else if (val > *(pNode->m_val))
			{
				//如果没有右孩子，则新节点成为为右孩子
				if (pNode->m_pRight == nullptr)
				{
					Node<T>* pNew = new Node<T>(val);
					pNode->m_pRight = pNew;
					pNew->m_pParent = pNode;

					UpdateHeight(pNew);
					//插入成功，结束
					break;
				}

				//到右子树查找
				pNode = pNode->m_pRight;
			}
			else
			{
				//相等，已经存在， 返回，暂时不支持同值
				return *this;
			}
		}

		return *this;
	}
	CAVL& Delete(T val)
	{
		Node<T>* pNode = Find(val);
		if (pNode != nullptr) {
			Node<T>* minPRight = Min(pNode->m_pRight);
			Node<T>* pParent = pNode->m_pParent;
			Node<T>* currentPNode = nullptr;
			Node<T>* pLeft = pNode->m_pLeft;
			Node<T>* pRight = pNode->m_pRight;
			if (minPRight != nullptr)
			{
				minPRight->m_pLeft = pLeft;
				if (pLeft != nullptr)
				{
					pLeft->m_pParent = minPRight;
				}
				if (minPRight->m_pParent != nullptr)
				{
					minPRight->m_pParent->m_pLeft = nullptr;
				}
				if (minPRight != pNode->m_pRight)
				{
					minPRight->m_pRight = pNode->m_pRight;
				}
				if (pRight != nullptr)
				{
					pRight->m_pParent = minPRight;
				}
				minPRight->m_pParent = pParent;
				currentPNode = minPRight;
			}
			else
			{
				if (pLeft != nullptr)
				{
					pNode->m_pLeft->m_pParent = pParent;
					currentPNode = pNode->m_pLeft;
				}
			}
			if (pParent != nullptr)
			{
				if (*(pNode->m_val) > *(pParent->m_val))
				{
					pParent->m_pRight = currentPNode;
				}
				else
				{
					pParent->m_pLeft = currentPNode;
				}
			}
			else
			{
				m_pRoot = currentPNode;
			}
			delete pNode;
			UpdateHeight(currentPNode);
		}
		return *this;
	} //删除
	CAVL& DeleteMin()
	{
		Node<T>* pMin = Min();
		if (pMin != nullptr)
		{
			Delete(*(pMin->m_val));
		}
		return *this;
	} //删除
	CAVL& DeleteMax()
	{
		Node<T>* pMax = Max();
		if (pMax != nullptr)
		{
			Delete(*(pMax->m_val));
		}
		return *this;
	} //删除
	CAVL& Update(T oldval, T newval)
	{
		Delete(oldval);
		Insert(newval);
		return *this;
	} //修改
	Node<T>* Find(T val)
	{
		Node<T>* pNode = m_pRoot;
		while (pNode != nullptr)
		{
			//判断
			if (val < *(pNode->m_val))
			{
				//到左子树查找
				pNode = pNode->m_pLeft;
			}
			else if (val > *(pNode->m_val))
			{
				//到右子树查找
				pNode = pNode->m_pRight;
			}
			else
			{
				return pNode;
			}
		}
		return nullptr;
	}//查询

	bool IsEmpty()
	{
		return m_pRoot == nullptr;
	} //判空
	void Clear()
	{
		while (!IsEmpty())
		{
			DeleteMin();
		}
	}  //清空
	Node<T>* Max(Node<T>* pNode) {
		if (pNode != nullptr) {
			while (pNode->m_pRight != nullptr)
			{
				pNode = pNode->m_pRight;
			}
		}
		return pNode;
	}
	Node<T>* Max() {
		return Max(m_pRoot);
	}
	Node<T>* Min(Node<T>* pNode)
	{
		if (pNode != nullptr) {
			while (pNode->m_pLeft != nullptr)
			{
				pNode = pNode->m_pLeft;
			}
		}
		return pNode;
	}
	Node<T>* Min()
	{
		return Min(m_pRoot);
	}
	void TraverseMid()
	{
		Node<T>* pNode = m_pRoot;
		CStack<Node<T>*> stack;
		do
		{
			while (pNode != nullptr)
			{
				stack.Push(pNode);
				pNode = pNode->m_pLeft;
			}
			Node<T>* currentNode = stack.Pop();
			std::cout << *(currentNode->m_val) << std::endl;
			pNode = currentNode->m_pRight;

		} while (!stack.Empty() || pNode != nullptr);
	}
	void TraverseFront()
	{
		Node<T>* pNode = m_pRoot;
		CStack<Node<T>*> stack;
		while (!stack.Empty() || pNode != nullptr)
		{
			if (pNode != nullptr) {
				std::cout << *(pNode->m_val) << std::endl;
				stack.Push(pNode->m_pRight);
				stack.Push(pNode->m_pLeft);
			}
			pNode = stack.Pop();
		}
	}
	void TraverseBack()
	{
		Node<T>* pNode = m_pRoot;
		Node<T>* lastNode = nullptr;
		CStack<Node<T>*> stack;
		do
		{
			while (pNode != nullptr)
			{
				stack.Push(pNode);
				pNode = pNode->m_pLeft;
			}
			Node<T>* currentNode = stack.Pop();
			if (currentNode->m_pRight == nullptr || currentNode->m_pRight == lastNode)
			{
				std::cout << *(currentNode->m_val) << std::endl;
				lastNode = currentNode;
			}
			else
			{
				stack.Push(currentNode);
				pNode = currentNode->m_pRight;
			}

		} while (!stack.Empty() || pNode != nullptr);
	}
	void TraverseLayer()
	{
		CQueue<Node<T>*> queue;
		queue.Push(m_pRoot);
		while (!queue.Empty())
		{
			Node<T>* pNode = queue.Pop();
			std::cout << *(pNode->m_val) << "(" << pNode->m_nHeight << ")" << std::endl;
			if (pNode->m_pLeft)
			{
				queue.Push(pNode->m_pLeft);
			}
			if (pNode->m_pRight)
			{
				queue.Push(pNode->m_pRight);
			}
		}
	}

private:
	void CopyDeep(const CAVL& bstTree)
	{
		CQueue<Node<T>*> queue;
		queue.Push(m_pRoot);
		while (!queue.Empty())
		{
			Node<T>* pNode = queue.Pop();
			Insert(*(pNode->m_val));
			if (pNode->m_pLeft)
			{
				queue.Push(pNode->m_pLeft);
			}
			if (pNode->m_pRight)
			{
				queue.Push(pNode->m_pRight);
			}
		}
	}
	void UpdateHeight(Node<T>* pNode)
	{
		while (pNode != nullptr)
		{
			pNode->m_nHeight = CalculateHeight(pNode);
			int nLeftHeight = GetHeight(pNode->m_pLeft);
			int nRightHeight = GetHeight(pNode->m_pRight);
			if (nLeftHeight < nRightHeight - 1)
			{
				Node<T>* pR = pNode->m_pRight;
				if (GetHeight(pR->m_pRight) >= GetHeight(pR->m_pLeft))
				{
					LeftRotation(pNode);
				}
				else
				{
					RightRotation(pR);
					LeftRotation(pNode);

				}
			}
			if (nRightHeight < nLeftHeight - 1)
			{
				Node<T>* pL = pNode->m_pLeft;
				if (GetHeight(pL->m_pLeft) >= GetHeight(pL->m_pRight))
				{
					RightRotation(pNode);
				}
				else
				{
					LeftRotation(pL);
					RightRotation(pNode);

				}
			}
			pNode = pNode->m_pParent;
		}
	}
	void LeftRotation(Node<T>* pNode)
	{
		auto pK = pNode;
		auto pP = pK->m_pParent;
		auto pKr = pK->m_pRight;
		auto pKrl = pKr->m_pLeft;
		auto pKrr = pKr->m_pRight;

		if (pP == nullptr)
		{
			m_pRoot = pKr;
		}
		else {
			if (pP->m_pLeft == pK)
			{
				pP->m_pLeft = pKr;
			}
			else
			{
				pP->m_pRight = pKr;
			}
		}

		pK->m_pParent = pKr;
		pK->m_pRight = pKrl;

		pKr->m_pParent = pP;
		pKr->m_pLeft = pK;

		if (pKrl != nullptr)
		{
			pKrl->m_pParent = pK;
		}

		pK->m_nHeight = CalculateHeight(pK);
		pKr->m_nHeight = CalculateHeight(pKr);

	}
	void RightRotation(Node<T>* pNode)
	{
		auto pK = pNode;
		auto pP = pK->m_pParent;
		auto pKl = pK->m_pLeft;
		auto pKll = pKl->m_pLeft;
		auto pKlr = pKl->m_pRight;

		if (pP == nullptr)
		{
			m_pRoot = pKl;
		}
		else {
			if (pP->m_pLeft == pK)
			{
				pP->m_pLeft = pKl;
			}
			else
			{
				pP->m_pRight = pKl;
			}
		}

		pK->m_pParent = pKl;
		pK->m_pLeft = pKlr;

		pKl->m_pParent = pP;
		pKl->m_pRight = pK;

		if (pKlr != nullptr)
		{
			pKlr->m_pParent = pK;
		}

		pK->m_nHeight = CalculateHeight(pK);
		pKl->m_nHeight = CalculateHeight(pKl);
	}
	size_t CalculateHeight(Node<T>* pNode)
	{
		return std::max(GetHeight(pNode->m_pLeft), GetHeight(pNode->m_pRight)) + 1;
	}
	size_t GetHeight(Node<T>* pNode)
	{
		return pNode != nullptr ? pNode->m_nHeight : 0;
	}
private:
	Node<T>* m_pRoot = nullptr; //根节点
};

