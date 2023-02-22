// ReferenceCount.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

/*
浅拷贝： 共享，重复释放
深拷贝： 内存冗余（重复），效率低
写时拷贝：

垃圾回收
*/

class CInt
{
public:
    CInt() :m_pnCount(nullptr), m_pnVal(nullptr) {}
    CInt(int n)
    {
        m_pnVal = new int(n);
        m_pnCount = new int(1);
    }
    CInt(const CInt& obj) //拷贝构造
    {
        //浅拷贝
        m_pnVal = obj.m_pnVal;
        m_pnCount = obj.m_pnCount;

        if(m_pnCount != nullptr)
        {
            //计数值加1
            *m_pnCount += 1;
        }
    }
    ~CInt()
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
    void Set(int n)
    {
        if (m_pnVal == nullptr)
        {
            m_pnVal = new int(n);
            m_pnCount = new int(1);
        }
        else if (m_pnVal != nullptr && *m_pnCount == 1) {
            *m_pnVal = n;
        }
        else
        {
            //申请新的内存和配套计数值
            int* pnNewVal = new int(n);
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
    int* m_pnVal;
    int* m_pnCount;
};

int main()
{
    CInt n(0);
    CInt n1 = n;
    CInt n2 = n1;
    CInt n3 = n;

    //测试，并修复bug
    n.Set(0x11111111);
    n.Set(0x22222222);

    CInt n4;
    n4.Set(0x9999);

    CInt n5;
    CInt n6 = n5;

    CInt n7(10);
    n7.Set(12);
    n7.Set(13);
    n7.Set(14);

    return 0;
}

