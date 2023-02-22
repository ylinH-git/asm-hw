#include "CAVL.h"
#include <iostream>
#include "CStack.h"
#include "CQueue.h"
CAVL::CAVL(): m_pRoot(nullptr)
{}

CAVL::CAVL(const CAVL& bstTree): m_pRoot(nullptr)
{
    CopyDeep(bstTree);
}
void CAVL::CopyDeep(const CAVL& bstTree)
{
    CQueue<Node*> queue;
    queue.Push(m_pRoot);
    while (!queue.Empty())
    {
        Node* pNode = queue.Pop();
        Insert(pNode->m_val);
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
void CAVL::UpdateHeight(Node* pNode)
{
    Node* insertNode = pNode;
    while (pNode != nullptr)
    {
        Node* currentNode = pNode;
        pNode->m_nHeight = std::max(GetHeight(pNode->m_pLeft), GetHeight(pNode->m_pRight)) + 1;
        pNode = pNode->m_pParent;
        if (pNode != nullptr)
        {
            int nLeftHeight = GetHeight(pNode->m_pLeft);
            int nRightHeight = GetHeight(pNode->m_pRight);
            if (nLeftHeight < nRightHeight - 1)
            {
                if (currentNode->m_pLeft == insertNode)
                {
                    RightRotation(currentNode);
                    pNode = currentNode;
                }
                else
                {
                    LeftRotation(pNode);
                }
            }
            if (nRightHeight < nLeftHeight - 1)
            {
                if (currentNode->m_pRight == insertNode)
                {
                    LeftRotation(currentNode);
                    pNode = currentNode;
                }
                else
                {
                    RightRotation(pNode);
                }
            }
        }
    }
}
void CAVL::LeftRotation(Node* pNode)
{
    Node* pParent = pNode->m_pParent;
    Node* pRight = pNode->m_pRight;

    pNode->m_pRight = pRight->m_pLeft;
    pRight->m_pLeft = pNode;
    if (pParent != nullptr)
    {
        if (pRight->m_val < pParent->m_val)
        {
            pParent->m_pLeft = pRight;
        }
        else
        {
            pParent->m_pRight = pRight;
        }

    }
    pRight->m_pParent = pParent;
    pNode->m_pParent = pRight;
    if (pNode == m_pRoot)
    {
        m_pRoot = pRight;
    }
}
void CAVL::RightRotation(Node* pNode)
{
    Node* pParent = pNode->m_pParent;
    Node* pLeft = pNode->m_pLeft;

    pNode->m_pLeft = pLeft->m_pRight;
    pLeft->m_pRight = pNode;
    if (pParent != nullptr)
    {
        if (pLeft->m_val < pParent->m_val)
        {
            pParent->m_pLeft = pLeft;
        }
        else
        {
            pParent->m_pRight = pLeft;
        }

    }
    pLeft->m_pParent = pParent;
    pNode->m_pParent = pLeft;
    if (pNode == m_pRoot)
    {
        m_pRoot = pLeft;
    }
}
size_t CAVL::GetHeight(Node* pNode)
{
    return pNode != nullptr ? pNode->m_nHeight : 0;
}
CAVL::CAVL(CAVL&& bstTree) noexcept : m_pRoot(nullptr)
{
    m_pRoot = bstTree.m_pRoot;
    bstTree.m_pRoot = nullptr;
}
CAVL& CAVL::operator=(const CAVL& bstTree)
{
    CopyDeep(bstTree);
    return *this;
}
CAVL::~CAVL()
{
    Clear();
}
CAVL& CAVL::Insert(int val)
{
  //�п�
  if (IsEmpty())
  {
    m_pRoot = new Node(val); //�½ڵ���Ϊ�����ĵ�һ���ڵ�(���ڵ�)
    return *this;
  }

  //�Ӹ��ڵ㿪ʼ����λ��
  Node* pNode = m_pRoot;
  while (pNode != nullptr)
  {
    //�ж�
    if (val < pNode->m_val)
    {
      //���û�����ӣ����½ڵ��ΪΪ����
      if (pNode->m_pLeft == nullptr)
      {
        Node* pNew = new Node(val);
        pNode->m_pLeft = pNew;
        pNew->m_pParent = pNode;

        UpdateHeight(pNew);
        //����ɹ�������
        break;
      }

      //������������
      pNode = pNode->m_pLeft;
    }
    else if(val > pNode->m_val)
    {
      //���û���Һ��ӣ����½ڵ��ΪΪ�Һ���
      if (pNode->m_pRight == nullptr)
      {
        Node* pNew = new Node(val);
        pNode->m_pRight = pNew;
        pNew->m_pParent = pNode;

        UpdateHeight(pNew);
        //����ɹ�������
        break;
      }

      //������������
      pNode = pNode->m_pRight;
    }
    else
    {
      //��ȣ��Ѿ����ڣ� ���أ���ʱ��֧��ֵͬ
      return *this;
    }
  }

  return *this;
}

CAVL& CAVL::Delete(int val)
{
    Node* pNode = Find(val);
    if (pNode != nullptr) {
        Node* minPRight = Min(pNode->m_pRight);
        Node* pParent = pNode->m_pParent;
        Node* currentPNode = nullptr;
        Node* pLeft = pNode->m_pLeft;
        Node* pRight = pNode->m_pRight;
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
            if (pLeft !=nullptr)
            {
                pNode->m_pLeft->m_pParent = pParent;
                currentPNode = pNode->m_pLeft;
            }
        }
        if (pParent != nullptr)
        {
            if (pNode->m_val > pParent->m_val)
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
        UpdateHeight(currentPNode);
        delete pNode;
    }
    return *this;
}

CAVL& CAVL::DeleteMin()
{
    Node* pMin = Min();
    if (pMin != nullptr)
    {
        Delete(pMin->m_val);
    }
    return *this;
}

CAVL& CAVL::DeleteMax()
{
    Node* pMax = Max();
    if (pMax != nullptr)
    {
        Delete(pMax->m_val);
    }
    return *this;
}

CAVL& CAVL::Update(int oldval, int newval)
{
    Delete(oldval);
    Insert(newval);
    return *this;
}

Node* CAVL::Find(int val)
{
    Node* pNode = m_pRoot;
    while (pNode != nullptr)
    {
        //�ж�
        if (val < pNode->m_val)
        {
            //������������
            pNode = pNode->m_pLeft;
        }
        else if (val > pNode->m_val)
        {
            //������������
            pNode = pNode->m_pRight;
        }
        else
        {
            return pNode;
        }
    }
    return nullptr;
}

bool CAVL::IsEmpty()
{
  return m_pRoot == nullptr;
}

void CAVL::Clear()
{
    while (!IsEmpty())
    {
        DeleteMin();
    }
}

Node* CAVL::Max(Node* root)
{
    if (root == nullptr) {
        return nullptr;
    }
    if (root->m_pLeft != nullptr)
    {
        return Max(root->m_pRight);
    }
    return root;
}

Node* CAVL::Max()
{
    if (m_pRoot == nullptr)
    {
        return nullptr;
    }
    if (m_pRoot->m_pRight != nullptr)
    {
        return Max(m_pRoot->m_pRight);
    }
    return m_pRoot;
}

Node* CAVL::Min(Node* root)
{
    if (root == nullptr) {
        return nullptr;
    }
    if (root->m_pLeft != nullptr)
    {
        return Max(root->m_pLeft);
    }
    return root;
}

Node* CAVL::Min()
{
    if (m_pRoot == nullptr)
    {
        return nullptr;
    }
    if (m_pRoot->m_pLeft != nullptr)
    {
        return Max(m_pRoot->m_pLeft);
    }
    return m_pRoot;
}

void CAVL::TraverseMid()
{
    Node* pNode = m_pRoot;
    CStack<Node*> stack;
    do
    {
        while (pNode != nullptr)
        {
            stack.Push(pNode);
            pNode = pNode->m_pLeft;
        }
        Node* currentNode = stack.Pop();
        std::cout << currentNode->m_val << std::endl;
        pNode = currentNode->m_pRight;

    } while (!stack.Empty() || pNode != nullptr);
}

void CAVL::TraverseFront()
{
    Node* pNode = m_pRoot;
    CStack<Node*> stack;
    while (!stack.Empty() || pNode != nullptr)
    {
        if (pNode != nullptr) {
            std::cout << pNode->m_val << std::endl;
            stack.Push(pNode->m_pRight);
            stack.Push(pNode->m_pLeft);
        }
        pNode = stack.Pop();
    }
}

void CAVL::TraverseBack()
{
    Node* pNode = m_pRoot;
    Node* lastNode = nullptr;
    CStack<Node*> stack;
    do
    {
        while (pNode != nullptr)
        {
            stack.Push(pNode);
            pNode = pNode->m_pLeft;
        }
        Node* currentNode = stack.Pop();
        if (currentNode->m_pRight == nullptr || currentNode->m_pRight == lastNode)
        {
            std::cout << currentNode->m_val << std::endl;
            lastNode = currentNode;
        }
        else
        {
            stack.Push(currentNode);
            pNode = currentNode->m_pRight;
        }

    } while (!stack.Empty() || pNode != nullptr);
}

void CAVL::TraverseLayer()
{
    CQueue<Node*> queue;
    queue.Push(m_pRoot);
    while (!queue.Empty())
    {
        Node* pNode = queue.Pop();
        std::cout << pNode->m_val << "(" << pNode->m_nHeight << ")" << std::endl;
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
