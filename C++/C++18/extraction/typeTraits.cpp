// typeTraits.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <functional>
template<typename T>
struct is_array
{
    const static bool value = false;
};

template<typename T>
struct is_array<T[]>
{
    const static bool value = true;
};

template<typename T, int nSize>
struct is_array<T[nSize]>
{
    const static bool value = true;
};

template<typename T1, typename T2>
struct is_same
{
    const static bool value = false;
};

template<typename T>
struct is_same<T, T>
{
    const static bool value = true;
};

template<typename T>
struct is_pointer
{
    const static bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
    const static bool value = true;
};

template<typename T>
struct is_reference
{
    const static bool value = false;
};

template<typename T>
struct is_reference<T&>
{
    const static bool value = true;
};

template<typename T>
struct is_arithmetic
{
    const static bool value = false;
};

template<>
struct is_arithmetic<int>
{
    const static bool value = true;
};

template<>
struct is_arithmetic<char>
{
    const static bool value = true;
};

template<>
struct is_arithmetic<short>
{
    const static bool value = true;
};

template<typename T>
struct is_member_pointer
{
    const static bool value = false;
};

template<typename T>
struct is_member_pointer<void (T::*)(int)>
{
    const static bool value = true;
};

class CTest
{
public:
    void test(int x) {};
    int m_n = 0;
};

template<typename T>
struct is_function
{
    const static bool value = false;
};

template<>
struct is_function<void ()>
{
    const static bool value = true;
};

template<typename T>
struct is_signed
{
    const static bool value = true;
};

template<>
struct is_signed<unsigned>
{
    const static bool value = false;
};

template<>
struct is_signed<unsigned short>
{
    const static bool value = false;
};


template<>
struct is_signed<unsigned char>
{
    const static bool value = false;
};

template<typename T>
struct remove_extent { typedef T type; };

template<typename T>
struct remove_extent<T[]> { typedef T type; };

template<typename T, std::size_t N>
struct remove_extent<T[N]> { typedef T type; };

template<typename T>
struct remove_reference { typedef T type; };

template<typename T>
struct remove_reference<T&> { typedef T type; };

template<typename T>
struct function_traits;

template<typename retT, typename arg1T, typename arg2T>
struct function_traits<retT(arg1T, arg2T)>
{
    typedef retT rettype;
    typedef arg1T arg1type;
    typedef arg2T arg2type;
};
void test() {}
int main()
{
    std::cout << std::boolalpha << is_array<int[]>::value << std::endl;
    std::cout << std::boolalpha << is_same<int, int>::value << std::endl;
    std::cout << std::boolalpha << is_pointer<int*>::value << std::endl;
    std::cout << std::boolalpha << is_reference<int&>::value << std::endl;
    std::cout << std::boolalpha << is_arithmetic<char>::value << std::endl;
    std::cout << std::boolalpha << is_member_pointer<decltype(&CTest::test)>::value << std::endl;
    std::cout << std::boolalpha << is_function<decltype(test)>::value << std::endl;
    std::cout << std::boolalpha << is_signed<int>::value << std::endl;;
    std::cout << (remove_extent<int[]>::type)(3.6) << std::endl;
    std::cout << (remove_reference<int&>::type)(3.6) << std::endl;
    std::cout << typeid(function_traits<void(double, double)>::rettype).name() << std::endl;
    std::cout << typeid(function_traits<void(double, double)>::arg1type).name() << std::endl;
    std::cout << typeid(function_traits<void(double, double)>::arg2type).name() << std::endl;
}

