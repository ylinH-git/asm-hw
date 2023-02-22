#pragma once

struct  Node
{
  Node(int val):m_val(val){}
  int m_val;                 //����
  Node* m_pParent = nullptr; //���ڵ�
  Node* m_pLeft   = nullptr; //����
  Node* m_pRight  = nullptr; //�Һ���
};

class CBinarySearchTree
{
public:
  //������

  CBinarySearchTree& Insert(int val); //����
  CBinarySearchTree& Delete(int val); //ɾ��
  CBinarySearchTree& DeleteMin(); //ɾ��
  CBinarySearchTree& DeleteMax(); //ɾ��
  CBinarySearchTree& Update(int oldval, int newval); //�޸�
  Node* Find(int val) ;//��ѯ

  bool IsEmpty(); //�п�
  void Clear();   //���
  Node* Max(Node* root);
  Node* Max();
  Node* Min(Node* root);
  Node* Min();

private:
  Node* m_pRoot = nullptr; //���ڵ�

};

