// CPoly.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#define GET_OFFSET(s, m) (size_t)&((struct s *)NULL)->m

void VirtualSayHello() 
{
    printf("sayhello, person");
}
void  VirtualSayGoodBye()
{
    printf("saygoodbye, person");
}

void SayHello()
{
    printf("sayhello, student");
}
void SayGoodBye()
{
    printf("saygoodbye, student");
}
struct SPerson {
    void* vsTable[2];
    void* __vsptr;
};

struct SStudent {
    void* vsTable[2];
    void* __vsptr;
};

void ConstructPerson(struct SPerson* person)
{
    person->vsTable[0] = (void *)VirtualSayHello;
    person->vsTable[1] = (void*)VirtualSayGoodBye;
    person->__vsptr = person->vsTable;
}

void ConstructStudent(struct SStudent* student)
{
    int i = 0;
    void* vsTable[2] = { (void*)SayHello , (void*)SayGoodBye };
    ConstructPerson(student);
    for (i = 0; i < sizeof(student->vsTable) / sizeof(void*); i++)
    {
        if (student->vsTable[i] != vsTable[i]) {
            student->vsTable[i] = vsTable[i];
        }
    }
    student->__vsptr = student->vsTable;
}

void RunFunc(struct SPerson* person)
{
    (*(void(*) ())(person->vsTable[0]))();
    (*(void(*) ())(person->vsTable[1]))();
}
int main()
{
    struct SStudent* sStu = malloc(sizeof(struct SStudent));
    ConstructStudent(sStu);
    RunFunc(sStu);
    free(sStu);
    return 0;
}

