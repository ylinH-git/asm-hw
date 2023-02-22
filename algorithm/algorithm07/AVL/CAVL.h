#pragma once

struct  Node
{
	Node(int val):m_val(val){}
	int m_val;                 //数据
	Node* m_pParent = nullptr; //父节点
	Node* m_pLeft   = nullptr; //左孩子
	Node* m_pRight  = nullptr; //右孩子
	size_t m_nHeight = 1;
};

class CAVL
{
public:
  //六件套
	CAVL();
	CAVL(const CAVL& bstTree);
	CAVL(CAVL&& bstTree) noexcept;
	CAVL& operator=(const CAVL& bstTree);
	CAVL& operator=(CAVL&& bstTree) = delete;
	~CAVL();

	CAVL& Insert(int val); //插入
	CAVL& Delete(int val); //删除
	CAVL& DeleteMin(); //删除
	CAVL& DeleteMax(); //删除
	CAVL& Update(int oldval, int newval); //修改
	Node* Find(int val) ;//查询

	bool IsEmpty(); //判空
	void Clear();   //清空
	Node* Max(Node* root);
	Node* Max();
	Node* Min(Node* root);
	Node* Min();
	void TraverseMid();
	void TraverseFront();
	void TraverseBack();
	void TraverseLayer();

private:
	void CopyDeep(const CAVL& bstTree);
	void UpdateHeight(Node* pNode);
	void LeftRotation(Node* pNode);
	void RightRotation(Node* pNode);
	size_t GetHeight(Node* pNode);
private:
	Node* m_pRoot = nullptr; //根节点
};

