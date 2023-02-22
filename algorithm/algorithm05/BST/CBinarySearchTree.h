#pragma once

struct  Node
{
  Node(int val):m_val(val){}
  int m_val;                 //数据
  Node* m_pParent = nullptr; //父节点
  Node* m_pLeft   = nullptr; //左孩子
  Node* m_pRight  = nullptr; //右孩子
};

class CBinarySearchTree
{
public:
  //六件套

  CBinarySearchTree& Insert(int val); //插入
  CBinarySearchTree& Delete(int val); //删除
  CBinarySearchTree& DeleteMin(); //删除
  CBinarySearchTree& DeleteMax(); //删除
  CBinarySearchTree& Update(int oldval, int newval); //修改
  Node* Find(int val) ;//查询

  bool IsEmpty(); //判空
  void Clear();   //清空
  Node* Max(Node* root);
  Node* Max();
  Node* Min(Node* root);
  Node* Min();

private:
  Node* m_pRoot = nullptr; //根节点

};

