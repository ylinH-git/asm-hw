// shareptr.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <memory>
using namespace std;
class CInt
{
public:
    CInt()
    {
        cout << "CInt" << endl;
    }
    ~CInt()
    {
        cout << "~CInt" << endl;
    }
};
template<class T> 
class share_ptr
{
public:
    share_ptr() :m_pnCount(nullptr), m_pnVal(nullptr) {}
    share_ptr(T* n)
    {
        m_pnVal = n;
        m_pnCount = new int(1);
    }
    share_ptr(const share_ptr& obj) //拷贝构造
    {
        //浅拷贝
        m_pnVal = obj.m_pnVal;
        m_pnCount = obj.m_pnCount;

        if (m_pnCount != nullptr)
        {
            //计数值加1
            *m_pnCount += 1;
        }
    }
     ~share_ptr()
    {
        if (m_pnCount == nullptr)
        {
            if (m_pnVal != nullptr)
            {
                delete m_pnVal;
            }
        }
        else if (*m_pnCount == 1)
        {
            //计数值为1， 只有一个对象持有内存（资源）
            delete m_pnVal;
            delete m_pnCount;
        }
        else
        {
            //计数值不为1，除了自己还有其它对象公用内存，只减少计数，不释放内存（资源）
            *m_pnCount -= 1;
        }
    }
    void Set(T* n)
    {
        if (m_pnVal == nullptr)
        {
            m_pnVal = n;
            m_pnCount = new int(1);
        }
        else if (m_pnVal != nullptr && *m_pnCount == 1) {
            *m_pnVal = n;
        }
        else
        {
            //申请新的内存和配套计数值
            int* pnNewVal = n;
            int* pnNewCnt = new int(1);

            //原来的计数值减一
            *m_pnCount -= 1;

            //保存新的内存和计数值
            m_pnVal = pnNewVal;
            m_pnCount = pnNewCnt;
        }
    }
    int Get()
    {
        return *m_pnVal;
    }
private:
    T* m_pnVal;
    int* m_pnCount;
};

template<class T>
class share_ptr<T[]>
{
public:
    share_ptr() :m_pnCount(nullptr), m_pnVal(nullptr) {}
    share_ptr(T* n)
    {
        m_pnVal = n;
        m_pnCount = new int(1);
    }
    share_ptr(const share_ptr& obj) //拷贝构造
    {
        //浅拷贝
        m_pnVal = obj.m_pnVal;
        m_pnCount = obj.m_pnCount;

        if (m_pnCount != nullptr)
        {
            //计数值加1
            *m_pnCount += 1;
        }
    }
    ~share_ptr()
    {
        if (m_pnCount == nullptr)
        {
            if (m_pnVal != nullptr)
            {
                delete[] m_pnVal;
            }
        }
        else if (*m_pnCount == 1)
        {
            delete[] m_pnVal;
            delete m_pnCount;
        }
        else
        {
            //计数值不为1，除了自己还有其它对象公用内存，只减少计数，不释放内存（资源）
            *m_pnCount -= 1;
        }
    }
private:
    T* m_pnVal;
    int* m_pnCount;
};

int main()
{
    share_ptr<CInt[]> ptr(new CInt[5]);
    //shared_ptr<CInt[]> ptr(new CInt[5]);
    std::cout << "Hello World!\n";
}
