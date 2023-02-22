#include "CBinarySearchTree.h"
#include <iostream>
#include "CStack.h"
#include "CQueue.h"
CBinarySearchTree& CBinarySearchTree::Insert(int val)
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

CBinarySearchTree& CBinarySearchTree::Delete(int val)
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
        delete pNode;
    }
    return *this;
}

CBinarySearchTree& CBinarySearchTree::DeleteMin()
{
    Node* pMin = Min();
    Delete(pMin->m_val);
    return *this;
}

CBinarySearchTree& CBinarySearchTree::DeleteMax()
{
    Node* pMax = Max();
    Delete(pMax->m_val);
    return *this;
}

CBinarySearchTree& CBinarySearchTree::Update(int oldval, int newval)
{
    Node* pNode = Find(oldval);
    if (pNode != nullptr) {
        Node* pLeft = pNode->m_pLeft;
        Node* pRight = pNode->m_pRight;
        if (pLeft != nullptr && newval < pLeft->m_val)
        {
            int nVal = pLeft->m_val;
            Update(nVal, newval);
            pNode->m_val = pLeft->m_val;
        }
        else if (pRight != nullptr && newval > pRight->m_val)
        {
            int nVal = pRight->m_val;
            Update(nVal, newval);
            pNode->m_val = pRight->m_val;
        }
        else
        {
            pNode->m_val = newval;
        } 
    }
    return *this;
}

Node* CBinarySearchTree::Find(int val)
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

bool CBinarySearchTree::IsEmpty()
{
  return m_pRoot == nullptr;
}

void CBinarySearchTree::Clear()
{
    while (!IsEmpty())
    {
        DeleteMin();
    }
}

Node* CBinarySearchTree::Max(Node* root)
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

Node* CBinarySearchTree::Max()
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

Node* CBinarySearchTree::Min(Node* root)
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

Node* CBinarySearchTree::Min()
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

void CBinarySearchTree::TraverseMid()
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

void CBinarySearchTree::TraverseFront()
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

void CBinarySearchTree::TraverseBack()
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

void CBinarySearchTree::TraverseLayer()
{
    CQueue<Node*> queue;
    queue.Push(m_pRoot);
    while (!queue.Empty())
    {
        Node* pNode = queue.Pop();
        std::cout << pNode->m_val << std::endl;
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
