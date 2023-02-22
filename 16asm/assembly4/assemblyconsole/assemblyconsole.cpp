// assemblyconsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
using namespace std;

enum class e16BReg
{
    AX = 0b000,
    CX = 0b001,
    DX = 0b010,
    BX = 0b011,
    SP = 0b100,
    BP = 0b101,
    SI = 0b110,
    DI = 0b111,
};

enum class e8BReg
{
    AL = 0b000,
    CL = 0b001,
    DL = 0b010,
    BL = 0b011,
    AH = 0b100,
    CH = 0b101,
    DH = 0b110,
    BH = 0b111,
};

enum class e8BReg
{
    AL = 0b000,
    CL = 0b001,
    DL = 0b010,
    BL = 0b011,
    AH = 0b100,
    CH = 0b101,
    DH = 0b110,
    BH = 0b111,
};

enum class Segment
{
    ES = 0b00,
    CS = 0b01,
    SS = 0b10,
    DS = 0b11,
};

enum class eRm
{
    BX_SI_DISP = 0b000,
    BX_DI_DISP = 0b001,
    BP_SI_DISP = 0b010,
    BP_DI_DISP = 0b011,
    SI_DISP = 0b100,
    DI_DISP = 0b101,
    BP_DISP = 0b110,
    BX_DISP = 0b111,
};

enum class AssemblyMap
{
    mov_rm16_to_r16    = 0b1000101111, // eg. mov bx, ax
    mov_rm8_to_r8      = 0b1000101011, // eg. mov bl, al
    mov_rm16_from_r16  = 0b1000100111, // eg. mov ax, bx
    mov_rm8_from_r8    = 0b1000100011, // eg. mov al, bl
    //mov_rm16_to_r16   = 0b1000101110, // eg. mov al, [bx + si + 408b]
    //mov_rm8_to_r8     = 0b1000101001, // eg. mov ax, [bx + si - 75]
    //mov_rm0_to_r16    = 0b1000101100, // eg. mov ax, [bx + si]
    //mov_rm0_to_r8     = 0b1000101000, // eg. mov al, [bx + si]
    //mov_rm16_from_r16 = 0b1000100110, // eg. mov [bx + si + 408b], al
    //mov_rm8_from_r8   = 0b1000100001, // eg. mov [bx + si - 75], ax
    //mov_rm0_from_r16  = 0b1000100100, // eg. mov [bx + si], bx
    //mov_rm0_from_r8   = 0b1000100000, // eg. mov [bx + si], bl
};


string Disassembly(char* mc, long nLen) 
{

    return "";
}

int main()
{
    char mc[10] = { 0x8B, 0x80, 0x8B, 0x40 };
    std::cout << Disassembly(mc, 10);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
