#include<stdio.h>


int main()
{
  //01010110
  char ch = 0x56;
  ch &= 0xef; //01000110 ，按位与
  ch |= 0x10; //01010110， 按位或
  ch ^= ch;//清零

  //01010110 - 10101001 
  char ch0 = 0x56;
  ch0 = ~ch0;

  return 0;
}

#if 0
// 编译器 8 
struct tagFoo
{
  char    m_ch;     // +0
  float   m_f;      // +4
  short   m_s;      // +8 
  char    m_ary[9]; // +10
  double  m_dbl;    // +24
  char    m_ch0;    // +32
};//sizeof = 40

enum Flag
{
  F_TEST = 0x12345678
};
struct  tagTest
{
  Flag    m_flag;//+0
  char    m_ch; //+5
  tagFoo  m_foo;//+8
  short   m_s;  //48
};//sizeof = 56

int main()
{
  tagFoo foo = { '\x11', 3.14f, 0x3333, "\x44\x44\x44\x44\x44", 3.14 };
  tagFoo ayr[10];
  tagTest t = { F_TEST, '\x99', foo, 0x8888 };

  return 0;
}

#endif // 0



#if 0

typedef int(__stdcall* PFN)(int, float);

#if 0
int __stdcall Foo0(int, float)
{
  return 0;
}
int __stdcall Foo1(int, float)
{
  return 0;
}
int __stdcall Foo2(int, float, double)
{
  return 0;
}
int __stdcall Foo3(int, double)
{
  return 0;
}

#endif // 0

int main()
{
  //Foo1(6, 6.5f);
  //Foo0(3, 3.2f);
  //Foo2(5, 6.3f, 9.8);
  //Foo3(8, 3.6);

  //PFN pfn = Foo0;
  //pfn(6, 6.5f);
  //pfn = Foo1;
  //pfn(6, 6.5f);
  //pfn = (PFN)Foo2;
  //pfn(6, 6.5f);

  //int n = 0;
  //scanf("%d", &n);

  //FPN pfns[] = {Add, Del, Update};
  //pfns[n]();

  //switch (n)
  //{
  //case 0:
  //  //Add
  //  break;
  //case 1:
  //  //Del
  //  break;
  //  //Fix
  //case 2:
  //  //Update
  //  break;
  //default:
  //  break;
  //}

  return 0;
}
#endif // 0


#if 0
void Foo(int ary[10])//int*
{
  sizeof(ary);
  &ary;

  return;
}


int main(int argc, char* argv[])
{
  void (*pfnFoo)(int ary[10]) = Foo;
  pfnFoo(NULL);
  (*pfnFoo)(NULL);

#if 0
  int n = 9;
  int* pn = &n;
  //pn = &argc;
  //int** ppn = &pn;

  int* pn2 = &argc;

  //pn + pn2;

  *pn = 8;
  ++pn; // pn 后移一个int大小
  pn += 4; //pn 后移4个int  
#endif // 0

#if 0

  int ary[10] = { 0,1,2,3,4,5,6,7,8,9 };
  //ary[5] = 44;//下标运算
  //sizeof(ary);

  Foo(ary);

  int* p = ary;

  *ary = 11;
  *(ary + 2) = 12;

  p[3] = 99;

  //ary += 3;

  int nSize = sizeof(ary);
  int(*pAry)[10] = &ary; //数组指针 -> 指向的是数组

  //&9;  
#endif // 0

  short ary[2][4] = {
    {0x21, 0x34, 0x55, 0x66},
    {0x11, 0x30, 0x75, 0x16} };

  int ary0[5][2][6] = {};

  short(*p)[4] = ary; //short[4]
  int(*p0)[2][6] = ary0;//int[2][6]

  p0 = &ary0[0];//int[2][6]
  int(*p1)[6] = &ary0[0][0];//int[6]

  int ary2[3] = {};
  &ary2[0];

  //等价写法
  //int* p = ary2;
  //int* p00 = &ary2[0];

  printf("hello world");
}
#endif // 0
