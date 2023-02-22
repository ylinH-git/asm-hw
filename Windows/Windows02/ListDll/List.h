#pragma once

#ifdef LISTDLL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

struct DLLAPI CLinkNode
{
	CLinkNode(int val) :m_val(val) {}
	CLinkNode() :m_val() {}

	int   m_val;            //数据
	CLinkNode* m_prev = nullptr; //前驱结点的地址
	CLinkNode* m_next = nullptr; //后继结点的地址
};

class DLLAPI CLinkList
{
public:
	 CLinkList(class CLinkList&&);
	 CLinkList(class CLinkList const&);
	 CLinkList(void);
	 ~CLinkList(void);
	 CLinkList& operator=(class CLinkList&&);
	 CLinkList& operator=(class CLinkList const&);
	 void append(class CLinkList const&);
	 struct CLinkNode* back(void)const;
	 void clear(void);
	 bool empty(void)const;
	 void erese(struct CLinkNode*, struct CLinkNode*);
	 void erese(struct CLinkNode*);
	 struct CLinkNode* front(void)const;
	 struct CLinkNode* insert(struct CLinkNode*, class Integer&&);
	 struct CLinkNode* insert(struct CLinkNode*, class Integer const&);
	 void pop_back(void);
	 void pop_front(void);
	 void push_back(class Integer&&);
	 void push_back(class Integer const&);
	 void push_front(class Integer&&);
	 void push_front(class Integer const&);
	 void show(void);
	 unsigned int size(void)const;
private: 
	void init(void);
	size_t m_nSize = 0;
	CLinkNode* m_pHeadGuard = nullptr; //头哨兵
	CLinkNode* m_pTailGuard = nullptr; //尾哨兵
};