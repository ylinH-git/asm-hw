// CVarient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#if 0
#include <iostream>
using namespace std;
int main()
{
	std::cout << "Hello World!\n";
}
#endif // 0



#include<iostream>
#include<memory>
#include <type_traits>
using namespace std;
//没有完成，最大的难点是不知道如何继续下去了。
/*
1. 共享指针封装模板
   兼容单个堆对象和数组堆对象
   share_ptr<CInt> ptr(new CInt);
   share_ptr<CInt[]> ptr(new CInt[5]);
*/


class CInt
{
public:
    CInt() { cout << "CInt" << endl; }
    ~CInt() {
        cout << "~CInt" << endl;
    }
    void Set(int n) { m_n = n; }
    int Get()
    {
        return m_n;
    }
private:
    int m_n = 0x1234567;
};

template<typename T>
struct mremove_extent
{
    using Type = T;
};

template<typename T>
struct mremove_extent<T[]>
{
    using Type = T;
};


template <typename T>
class CPtr
{
public:
    using EleType = typename mremove_extent<T>::Type;
    CPtr()
    {
        m_pT = nullptr;
        m_pnCout = nullptr;
    }

    CPtr(EleType* ptr)
    {
        if (ptr == nullptr)
        {
            m_pT = nullptr;
            m_pnCout = nullptr;
            return;
        }
        m_pT = ptr;
        m_pnCout = new size_t(1);
    }

    //拷贝构造
    CPtr(const CPtr<T>& ptr)
    {
        copy(ptr);

    }

    CPtr& operator =(const CPtr& ptr)
    {
        copy(ptr);
        return *this;
    }


    ~CPtr()	 //析构
    {
        DefRef();
    }

    EleType* operator->()
    {
        return 	m_pT;
    }
    EleType& operator*()
    {
        return	*m_pT;
    }

    template <class _Ty2 = T, class _Elem = EleType, enable_if_t<is_array_v<_Ty2>, int> = 0>
    _Elem& operator[](size_t nIdx)
    {
        return m_pT[nIdx];
    }

    bool operator ==(EleType* p)
    {
        return 	m_pT == p;
    }
    bool operator ==(const CPtr& p)
    {
        return 	m_pT == p.m_pT;
    }
    bool operator !=(EleType* p)
    {
        return 	!(m_pT == p);
    }
    bool operator !=(const CPtr& p)
    {
        return 	!(m_pT == p.m_pT);
    }
private:
    void DefRef()
    {
        if (m_pnCout == nullptr)
        {
            return;
        }

        if (--(*m_pnCout) == 0)
        {
            //判断类型是否是数组
            if (std::is_array<T>::value)
            {
                delete[] m_pT;
            }
            else
            {
                delete m_pT;
            }
            delete	m_pnCout;
            m_pT = nullptr;
            m_pnCout = nullptr;
        }
    }
    void copy(const CPtr<T>& ptr)
    {
        //自己等于自己
        if (this == &ptr)
        {
            return;
        }
        //判断是不是空指针
        if (ptr.m_pT == nullptr)
        {

            if (m_pT != nullptr)
            {
                delete m_pT;
                delete	m_pnCout;
                m_pT = nullptr;
                m_pnCout = nullptr;
            }

            return;
        }
        //判断是否同一块内存
        if (m_pT == ptr.m_pT)
        {
            return;
        }

        //三个指针指向两块内存，先实例化ptr,然后ptr1=ptr，再实例化ptr2，然后ptr1=ptr2
        //原来的计数值要-1
        DefRef();

        m_pT = ptr.m_pT;
        m_pnCout = ptr.m_pnCout;
        ++(*m_pnCout);
    }

private:
    EleType* m_pT;
    size_t* m_pnCout; //引用计数
};

int main()
{
    CPtr<CInt[]> ptr0(new CInt[3]);
    ptr0[2].Set(7);
}
