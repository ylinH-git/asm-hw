// assemblyconsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include "assemblyconsole.h"
using namespace std;



enum class eDirection
{
    from = 0,
    to = 1
};

enum class eWidth
{
    sixteen = 0,
    eight = 1
};

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

enum class eSegment
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

enum class eMod
{
    reg = 0b11,
    disp_0 = 0b00,
    disp_low_extended = 0b01,
    disp_high_low = 0b10,
};
enum class AssemblyMap
{
    mov_rm16_to_r16    = 0b10001011, 
    mov_rm8_to_r8      = 0b10001010,
    mov_rm16_from_r16  = 0b10001001,
    mov_rm8_from_r8    = 0b10001000,
    mov_imm_to_rm16    = 0b11000111,
    mov_imm_to_rm8     = 0b11000110,
    mov_mem_to_acc16   = 0b10100001,
    mov_mem_from_acc16 = 0b10100011,
    mov_mem_to_acc8    = 0b10100000,
    mov_mem_from_acc8  = 0b10100010,
    mov_rm_to_seg      = 0b10001110,
    mov_rm_from_seg    = 0b10001100,
    push_rm_or_inc_16  = 0b11111111,
    mov_imm_to_r16     = 0b10111,
    mov_imm_to_r8      = 0b10110,
    push_reg           = 0b01010,
    push_or_pop_seg    = 0b000, 
};


void GetMovRmTOFR(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width);
void GetMovImmToRm(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width);
void GetMovImmToR(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width);
void GetMovMemTOFAcc(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width);
void GetMovRmTOFSeg(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir);

unsigned char GetBits(unsigned char byteCode, unsigned char nIdx, unsigned char nLen)
{
    unsigned char leftN = nIdx;
    byteCode &= ~(0xff << (8 - nIdx));
    return  byteCode >> (8 - nIdx - nLen);
}

string GetSeg(unsigned char seg)
{
    switch (seg)
    {
    case (unsigned char)eSegment::ES:
        return "es";
    case (unsigned char)eSegment::SS:
        return "ss";
    case (unsigned char)eSegment::DS:
        return "ds";
    case (unsigned char)eSegment::CS:
        return "cs";
    }
}

string GetReg16(unsigned char reg)
{
    switch (reg)
    {
    case (unsigned char)e16BReg::AX :
        return "ax";
    case (unsigned char)e16BReg::CX:
        return "cx";
    case (unsigned char)e16BReg::DX:
        return "dx";
    case (unsigned char)e16BReg::BX:
        return "bx";
    case (unsigned char)e16BReg::SP:
        return "sp";
    case (unsigned char)e16BReg::BP:
        return "bp";
    case (unsigned char)e16BReg::SI:
        return "si";
    case (unsigned char)e16BReg::DI:
        return "di";
    }
}
string GetReg8(unsigned char reg)
{
    switch (reg)
    {
    case (unsigned char)e8BReg::AL:
        return "al";
    case (unsigned char)e8BReg::CL:
        return "cl";
    case (unsigned char)e8BReg::DL:
        return "dl";
    case (unsigned char)e8BReg::BL:
        return "bl";
    case (unsigned char)e8BReg::AH:
        return "ah";
    case (unsigned char)e8BReg::CH:
        return "ch";
    case (unsigned char)e8BReg::DH:
        return "dh";
    case (unsigned char)e8BReg::BH:
        return "bh";
    }
}

string GetRm(unsigned char rm)
{
    switch (rm)
    {
    case (unsigned char)eRm::BX_SI_DISP:
    {
        return "bx + si";
    }
    case (unsigned char)eRm::BX_DI_DISP:
    {
        return "bx + di";
    }
    case (unsigned char)eRm::BP_SI_DISP:
    {
        return "bp + si";
    }
    case (unsigned char)eRm::BP_DI_DISP:
    {
        return "bp + di";
    }
    case (unsigned char)eRm::SI_DISP:
    {
        return "si";
    }
    case (unsigned char)eRm::DI_DISP:
    {
        return "di";
    }
    case (unsigned char)eRm::BP_DISP:
    {
        return "bp";
    }
    case (unsigned char)eRm::BX_DISP:
    {
        return "bx";
    }
    }
}

int Disassembly(unsigned char* mcs, long nLen, string& assembly)
{
    int readLen = 0;
    unsigned char mc = mcs[0];
    switch (mc)
    {
    case (unsigned char)AssemblyMap::mov_rm16_to_r16:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::to, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_rm8_to_r8:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::to, eWidth::eight);
        break;
    }
    case (unsigned char)AssemblyMap::mov_rm16_from_r16:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::from, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_rm8_from_r8:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::from, eWidth::eight);
        break;
    }
    case (unsigned char)AssemblyMap::mov_imm_to_rm16:
    {
        GetMovImmToRm(readLen, assembly, mcs, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_imm_to_rm8:
    {
        GetMovImmToRm(readLen, assembly, mcs, eWidth::eight);
        break;
    }
    case (unsigned char)AssemblyMap::mov_mem_to_acc16:
    {
        GetMovMemTOFAcc(readLen, assembly, mcs, eDirection::to, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_mem_from_acc16:
    {
        GetMovMemTOFAcc(readLen, assembly, mcs, eDirection::from, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_mem_to_acc8:
    {
        GetMovMemTOFAcc(readLen, assembly, mcs, eDirection::to, eWidth::eight);
        break;
    }
    case (unsigned char)AssemblyMap::mov_mem_from_acc8:
    {
        GetMovMemTOFAcc(readLen, assembly, mcs, eDirection::from, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_rm_to_seg:
    {
        GetMovRmTOFSeg(readLen, assembly, mcs, eDirection::to);
        break;
    }
    case (unsigned char)AssemblyMap::mov_rm_from_seg:
    {
        GetMovRmTOFSeg(readLen, assembly, mcs, eDirection::from);
        break;
    }
    case (unsigned char)AssemblyMap::push_rm_or_inc_16:
    {
        unsigned char type = GetBits(mcs[1], 3, 3);
        if (type == 0b000)
        {

        }
        else
        {

        }
    }
    }

    switch (mc >> 3)
    {
    case (unsigned char)AssemblyMap::mov_imm_to_r16:
    {
        GetMovImmToR(readLen, assembly, mcs, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_imm_to_r8:
    {
        GetMovImmToR(readLen, assembly, mcs,  eWidth::eight);
        break;
    }
    }
    return readLen;
}

void GetMovMemTOFAcc(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width)
{
    readLen += 1;
    assembly = "mov";
    string regStr = width == eWidth::sixteen ? "AX" : "AL";
    unsigned short* addr = (unsigned short*)&mcs[1];
    char addrStr[5] = {};
    snprintf(addrStr, 5, "%04x", *addr);

    if (dir == eDirection::to)
    {
        ((((assembly += " " )+= regStr) += ", [") += addrStr) += "]";
    }
    else
    {
        ((((assembly += " ") += "[") += addrStr) += "], ") += regStr;
    }

    readLen += 2;
}

void GetMovRmTOFR(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width)
{
    readLen += 1;
    assembly = "mov";
    unsigned char mod = GetBits(mcs[1], 0, 2);
    unsigned char reg = GetBits(mcs[1], 2, 3);
    unsigned char rm = GetBits(mcs[1], 5, 3);
    switch (mod)
    {
    case (unsigned char)eMod::reg:
    {
        string reg1Str = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        string reg2Str = width == eWidth::sixteen ? GetReg16(rm) : GetReg8(rm);
        if (dir == eDirection::to)
        {
            ((assembly += " ") += reg1Str += ", ") += reg2Str;
        }
        else
        {
            ((assembly += " ") += reg2Str += ", ") += reg1Str;
        }
        
        readLen += 1;
        break;
    }
    case (unsigned char)eMod::disp_0:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        if (rm != 0b110) {
            string rmStr = GetRm(rm);

            if (dir == eDirection::to)
            {
                (((assembly += " ") += regStr += ", [") += rmStr += " + ") += "]";
            }
            else
            {
                ((assembly += " [") += rmStr += "], ") += regStr;
            }
            readLen += 1;
        }
        else
        {
            unsigned short* disp = (unsigned short*)&mcs[2];
            char dispStr[5] = {};
            snprintf(dispStr, 5, "%04x", *disp);

            if (dir == eDirection::to)
            {
                (((assembly += " ") += regStr += ", [") += dispStr) += "]";
            }
            else
            {
                ((((assembly += " ") += "[") += dispStr) += "], ") += regStr;
            }
            readLen += 3;
        }
        
        break;
    }
    case (unsigned char)eMod::disp_low_extended:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        string rmStr = GetRm(rm);
        unsigned char* displow = &mcs[2];
        char dispStr[5] = {};
        if (*displow > 0x79)
        {
            snprintf(dispStr, 5, "ff%02x", *displow);
        }
        else
        {
            snprintf(dispStr, 5, "00%02x", *displow);
        }


        if (dir == eDirection::to)
        {
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr;
        }

      
        readLen += 2;
        break;
    }
    case (unsigned char)eMod::disp_high_low:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        string rmStr = GetRm(rm);
        unsigned short* disp = (unsigned short*)&mcs[2];
        char dispStr[5] = {};
        snprintf(dispStr, 5, "%04x", *disp);

        if (dir == eDirection::to)
        {
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr;
        }
        readLen += 3;
        break;
    }
    }
}

void GetPushRm(int& readLen, std::string& assembly, unsigned char* mcs)
{
    readLen += 1;
    assembly = "push";
    unsigned char mod = GetBits(mcs[1], 0, 2);
    unsigned char rm = GetBits(mcs[1], 5, 3);
    switch (mod)
    {
    case (unsigned char)eMod::reg:
    {
        string regStr = GetReg16(rm);
        (assembly += " ") += regStr;

        readLen += 1;
        break;
    }
    case (unsigned char)eMod::disp_0:
    {
        if (rm != 0b110) {
            string rmStr = GetRm(rm);

            (assembly += " [") += rmStr += "]";
            readLen += 1;
        }
        else
        {
            unsigned short* disp = (unsigned short*)&mcs[2];
            char dispStr[5] = {};
            snprintf(dispStr, 5, "%04x", *disp);
            ((assembly += " [") += dispStr) += "]";
            readLen += 3;
        }

        break;
    }
    case (unsigned char)eMod::disp_low_extended:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        string rmStr = GetRm(rm);
        unsigned char* displow = &mcs[2];
        char dispStr[5] = {};
        if (*displow > 0x79)
        {
            snprintf(dispStr, 5, "ff%02x", *displow);
        }
        else
        {
            snprintf(dispStr, 5, "00%02x", *displow);
        }


        if (dir == eDirection::to)
        {
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr;
        }


        readLen += 2;
        break;
    }
    case (unsigned char)eMod::disp_high_low:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        string rmStr = GetRm(rm);
        unsigned short* disp = (unsigned short*)&mcs[2];
        char dispStr[5] = {};
        snprintf(dispStr, 5, "%04x", *disp);

        if (dir == eDirection::to)
        {
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr;
        }
        readLen += 3;
        break;
    }
    }
}

void GetMovImmToRm(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width)
{
    readLen += 1;
    assembly = width == eWidth::sixteen ? "mov word ptr" : "mov byte ptr";
    unsigned char mod = GetBits(mcs[1], 0, 2);
    unsigned char rm = GetBits(mcs[1], 5, 3);
    switch (mod)
    {
    case (unsigned char)eMod::reg:
    case (unsigned char)eMod::disp_0:
    {
        unsigned char* datalow = &mcs[2];
        unsigned short* data = (unsigned short*)&mcs[2];
        char dataStr[5] = {};
        if (width == eWidth::sixteen)
        {
            snprintf(dataStr, 5, "%04x", *data);
        }
        else
        {
            snprintf(dataStr, 5, "%02x", *datalow);
        }

        if (mod == (unsigned char)eMod::reg)
        {
            assembly = "mov";
            string regStr = width == eWidth::sixteen ? GetReg16(rm) : GetReg8(rm);
            (((assembly += " ") += regStr += ", ") += dataStr);
        }
        if (mod == (unsigned char)eMod::reg)
        {
            string rmStr = GetRm(rm);
            (((assembly += " [") += rmStr += "], ") += dataStr);
        }
        readLen += (width == eWidth::sixteen ? 3 : 2);
        break;
    }
    case (unsigned char)eMod::disp_low_extended:
    {
        string rmStr = GetRm(rm);

        unsigned char* displow = &mcs[2];
        char dispStr[5] = {};

        unsigned char* datalow = &mcs[3];
        unsigned short* data = (unsigned short*)&mcs[3];
        char dataStr[5] = {};
        if (width == eWidth::sixteen)
        {
            snprintf(dataStr, 5, "%04x", *data);
        }
        else
        {
            snprintf(dataStr, 5, "%02x", *datalow);
        }

        if (*displow > 0x79)
        {
            snprintf(dispStr, 5, "ff%02x", *displow);
        }
        else
        {
            snprintf(dispStr, 5, "00%02x", *displow);
        }


        ((((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += dataStr);


        readLen += (width == eWidth::sixteen ? 4 : 3);
        break;
    }
    case (unsigned char)eMod::disp_high_low:
    {
        string rmStr = GetRm(rm);
        unsigned short* disp = (unsigned short*)&mcs[2];
        char dispStr[5] = {};
        snprintf(dispStr, 5, "%04x", *disp);

        unsigned char* datalow = &mcs[4];
        unsigned short* data = (unsigned short*)&mcs[4];
        char dataStr[5] = {};
        if (width == eWidth::sixteen)
        {
            snprintf(dataStr, 5, "%04x", *data);
        }
        else
        {
            snprintf(dataStr, 5, "%02x", *datalow);
        }

        ((((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += dataStr);
        readLen += (width == eWidth::sixteen ? 5 : 4);
        break;
    }
    }
}

void GetMovImmToR(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width)
{
    readLen += 1;
    assembly = width == eWidth::sixteen ? "mov word ptr" : "mov byte ptr";
    unsigned char reg = GetBits(mcs[0], 5, 3);
    unsigned char* datalow = &mcs[1];
    unsigned short* data = (unsigned short*)&mcs[1];
    char dataStr[5] = {};
    if (width == eWidth::sixteen)
    {
        snprintf(dataStr, 5, "%04x", *data);
    }
    else
    {
        snprintf(dataStr, 5, "%02x", *datalow);
    }

    assembly = "mov";
    string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
    (((assembly += " ") += regStr += ", ") += dataStr);
    readLen += (width == eWidth::sixteen ? 2 : 1);
}

void GetMovRmTOFSeg(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir)
{
    readLen += 1;
    assembly = "mov";
    unsigned char mod = GetBits(mcs[1], 0, 2);
    unsigned char seg = GetBits(mcs[1], 3, 2);
    unsigned char rm = GetBits(mcs[1], 5, 3);
    string segStr = GetSeg(seg);
    switch (mod)
    {
    case (unsigned char)eMod::reg:
    {
        string reg2Str = GetReg16(rm);
        if (dir == eDirection::to)
        {
            ((assembly += " ") += segStr += ", ") += reg2Str;
        }
        else
        {
            ((assembly += " ") += reg2Str += ", ") += segStr;
        }

        readLen += 1;
        break;
    }
    case (unsigned char)eMod::disp_0:
    {
        string rmStr = GetRm(rm);

        if (dir == eDirection::to)
        {
            (((assembly += " ") += segStr += ", word ptr [") += rmStr += " + ") += "]";
        }
        else
        {
            ((assembly += " word ptr [") += rmStr += "], ") += segStr;
        }


        readLen += 1;
        break;
    }
    case (unsigned char)eMod::disp_low_extended:
    {
        string rmStr = GetRm(rm);
        unsigned char* displow = &mcs[2];
        char dispStr[5] = {};
        if (*displow > 0x79)
        {
            snprintf(dispStr, 5, "ff%02x", *displow);
        }
        else
        {
            snprintf(dispStr, 5, "00%02x", *displow);
        }


        if (dir == eDirection::to)
        {
            ((((assembly += " ") += segStr += ", word ptr [") += rmStr += " + ") += dispStr) += "]";
        }
        else
        {
            (((((assembly += " ") += "word ptr [") += rmStr += " + ") += dispStr) += "], ") += segStr;
        }


        readLen += 2;
        break;
    }
    case (unsigned char)eMod::disp_high_low:
    {
        string rmStr = GetRm(rm);
        unsigned short* disp = (unsigned short*)&mcs[2];
        char dispStr[5] = {};
        snprintf(dispStr, 5, "%04x", *disp);

        if (dir == eDirection::to)
        {
            ((((assembly += " ") += segStr += ", word ptr [") += rmStr += " + ") += dispStr) += "]";
        }
        else
        {
            (((((assembly += " ") += "word ptr [") += rmStr += " + ") += dispStr) += "], ") += segStr;
        }
        readLen += 3;
        break;
    }
    }
}

#define ARR_LEN 23
int main()
{
    unsigned char mcs[ARR_LEN] =
    { 0x8B, 0x80, 0x8B, 0x40, 0xC7, 0x82, 0xff, 0x10, 
        0x34, 0x12, 0xb8, 0x34, 0x12, 0xA1, 0x00, 0x10,
        0xA0, 0x00, 0x10, 0x8e, 0x80, 0x34, 0x12 };
    string assembly = "";
    int nLen = 0;
    int nCurrentPos = 0;
    while (true)
    {
        int restSize = ARR_LEN - nCurrentPos;
        if (restSize <= 0)
        {
            break;
        }
        int nRet =  Disassembly(mcs + nCurrentPos, restSize, assembly);
        nCurrentPos += nRet;
        if (nRet == 0)
        {
            break;
        }
        else
        {
            cout << assembly << endl;
            assembly = "";
        }
    }
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
