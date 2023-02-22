#pragma once

struct  Node
{
	Node(int val):m_val(val){}
	int m_val;                 //����
	Node* m_pParent = nullptr; //���ڵ�
	Node* m_pLeft   = nullptr; //����
	Node* m_pRight  = nullptr; //�Һ���
	size_t m_nHeight = 1;
};

class CAVL
{
public:
  //������
	CAVL();
	CAVL(const CAVL& bstTree);
	CAVL(CAVL&& bstTree) noexcept;
	CAVL& operator=(const CAVL& bstTree);
	CAVL& operator=(CAVL&& bstTree) = delete;
	~CAVL();

	CAVL& Insert(int val); //����
	CAVL& Delete(int val); //ɾ��
	CAVL& DeleteMin(); //ɾ��
	CAVL& DeleteMax(); //ɾ��
	CAVL& Update(int oldval, int newval); //�޸�
	Node* Find(int val) ;//��ѯ

	bool IsEmpty(); //�п�
	void Clear();   //���
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
	Node* m_pRoot = nullptr; //���ڵ�
};

