#pragma once
#include "CArray.h"

template<typename T>
class CStack {
public:
	CStack(): m_arr(CArray<T>()) {}
	CStack& Push(T val)
	{
		m_arr.InsertTail(val);
		return *this;
	}
	T Pop()
	{
		T tData = m_arr[m_arr.Size() - 1];
		m_arr.DeleteTail();
		return tData;
	}
	bool Empty()
	{
		return m_arr.IsEmpty();
	}
private:
	CArray<T> m_arr;
};