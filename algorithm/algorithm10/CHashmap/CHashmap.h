#pragma once
#include <functional>
#include <stdexcept>

#define HashMapSize 4

template<typename Key, typename Value>
class CHashmap
{
private:
	struct Node
	{
		Node() {};
		Node(Key key, Value val) : m_key(key), m_val(val) {};
		Key m_key;
		Value m_val;
		Node* m_next = nullptr;
	};
public:
	CHashmap() {}

	CHashmap(const CHashmap& map)
	{
		CopyDeep(map);
	}

	CHashmap(CHashmap&& map)
	{
		for (int i = 0; i < HashMapSize; i++)
		{
			m_arr[i] = map.m_arr[i];
			map.m_arr[i] = nullptr;
		}
		m_nCount = map.m_nCount;

		map.m_nCount = 0;
	}

	~CHashmap() {
		Clear();
	}

	CHashmap& operator=(const CHashmap& map)
	{
		if (&map == this)
		{
			return *this;
		}
		else
		{
			Clear();
			CopyDeep(map);
		}
		return *this;
	}

	CHashmap& operator=(CHashmap&& map)
	{
		for (int i = 0; i < HashMapSize; i++)
		{
			m_arr[i] = map.m_arr[i];
			map.m_arr[i] = nullptr;
		}
		m_nCount = map.m_nCount;

		map.m_nCount = 0;

		return *this;
	}

	CHashmap& Insert(Key key, Value val)
	{
		size_t nIdx = std::hash<Key>{}(key) % HashMapSize;

		Node* pNewNode = new Node(key, val);

		pNewNode->m_next = m_arr[nIdx];
		m_arr[nIdx] = pNewNode;

		++m_nCount;
		return *this;
	}

	CHashmap& Delete(Key key, Value val)
	{
		size_t nIdx = std::hash<Key>{}(key) % HashMapSize;
		auto pDelNode = Find(key, val);
		if (pDelNode != nullptr)
		{
			auto pHead = m_arr[nIdx];
			pDelNode->m_key = pHead->m_key;
			pDelNode->m_val = pHead->m_val;

			m_arr[nIdx] = pHead->m_next;

			delete pHead;
			--m_nCount;
		}
		return *this;
	}

	void Clear()
	{
		for (int i = 0; i < HashMapSize; i++)
		{
			while (m_arr[i] != nullptr)
			{
				auto pNextNode = m_arr[i]->m_next;
				delete m_arr[i];
				m_arr[i] = pNextNode;
			}
		}
		m_nCount = 0;
	}

	Node* Find(Key key, Value val)
	{
		size_t nIdx = std::hash<Key>{}(key) % HashMapSize;
		Node* pNode = m_arr[nIdx];

		while (pNode != nullptr)
		{
			if (pNode->m_key == key && pNode->m_val == val)
			{
				break;
			}
			pNode = pNode->m_next;
		}

		return pNode;
	}

	Node* Find(Key key)
	{
		size_t nIdx = std::hash<Key>{}(key) % HashMapSize;
		Node* pNode = m_arr[nIdx];

		while (pNode != nullptr)
		{
			if (pNode->m_key == key)
			{
				break;
			}
			pNode = pNode->m_next;
		}

		return pNode;
	}

	Value& operator[](Key key)
	{
		auto pNode = Find(key);
		if (pNode == nullptr)
		{
			throw std::out_of_range("Ô½½ç");
		}
		return pNode->m_val;
	}

	size_t Size()
	{
		return m_nCount;
	}

private:
	void CopyDeep(const CHashmap<Key, Value>& map)
	{
		for (int i = 0; i < HashMapSize; i++)
		{
			auto pNode = map.m_arr[i];
			while (pNode != nullptr)
			{
				Insert(pNode->m_key, pNode->m_val);
				pNode = pNode->m_next;
			}
		}
		m_nCount = map.m_nCount;
	}
private:
	Node* m_arr[HashMapSize] = {};
	size_t m_nCount = 0;
};