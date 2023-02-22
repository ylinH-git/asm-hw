#pragma once
#include "CArray.h"

template<typename T>
class CQueue {
public:
	CQueue() : m_arr(CArray<T>()) {}
	CQueue& Push(T val)
	{
		m_arr.InsertTail(val);
		return *this;
	}
	T Pop()
	{
		T tData = m_arr[0];
		m_arr.DeleteHead();
		return tData;
	}
	bool Empty()
	{
		return m_arr.IsEmpty();
	}
private:
	CArray<T> m_arr;
};