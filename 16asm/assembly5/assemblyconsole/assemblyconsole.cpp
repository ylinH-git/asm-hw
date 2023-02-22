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
    mov_imm_to_r16     = 0b10111,
    mov_imm_to_r8      = 0b10110,
};


void GetMovRmTOFR(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width);

unsigned char GetBits(unsigned char byteCode, unsigned char nIdx, unsigned char nLen)
{
    unsigned char leftN = nIdx;
    byteCode &= ~(0xff << (8 - nIdx));
    return  byteCode >> (8 - nIdx - nLen);
}

string GetReg16(unsigned char reg)
{
    switch (reg)
    {
    case (unsigned char)e16BReg::AX :
        return "AX";
    case (unsigned char)e16BReg::CX:
        return "CX";
    case (unsigned char)e16BReg::DX:
        return "DX";
    case (unsigned char)e16BReg::BX:
        return "BX";
    case (unsigned char)e16BReg::SP:
        return "SP";
    case (unsigned char)e16BReg::BP:
        return "BP";
    case (unsigned char)e16BReg::SI:
        return "SI";
    case (unsigned char)e16BReg::DI:
        return "DI";
    }
}
string GetReg8(unsigned char reg)
{
    switch (reg)
    {
    case (unsigned char)e8BReg::AL:
        return "AL";
    case (unsigned char)e8BReg::CL:
        return "CL";
    case (unsigned char)e8BReg::DL:
        return "DL";
    case (unsigned char)e8BReg::BL:
        return "BL";
    case (unsigned char)e8BReg::AH:
        return "AH";
    case (unsigned char)e8BReg::CH:
        return "CH";
    case (unsigned char)e8BReg::DH:
        return "DH";
    case (unsigned char)e8BReg::BH:
        return "BH";
    }
}

string GetRm(unsigned char rm)
{
    switch (rm)
    {
    case (unsigned char)eRm::BX_SI_DISP:
    {
        return "BX + SI";
    }
    case (unsigned char)eRm::BX_DI_DISP:
    {
        return "BX + DI";
    }
    case (unsigned char)eRm::BP_SI_DISP:
    {
        return "BP + SI";
    }
    case (unsigned char)eRm::BP_DI_DISP:
    {
        return "BP + DI";
    }
    case (unsigned char)eRm::SI_DISP:
    {
        return "SI";
    }
    case (unsigned char)eRm::DI_DISP:
    {
        return "DI";
    }
    case (unsigned char)eRm::BP_DISP:
    {
        return "BP";
    }
    case (unsigned char)eRm::BX_DISP:
    {
        return "BX";
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
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::from, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_imm_to_rm8:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::from, eWidth::eight);
        break;
    }
    }

    switch (mc >> 3)
    {
    case (unsigned char)AssemblyMap::mov_imm_to_r16:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::from, eWidth::sixteen);
        break;
    }
    case (unsigned char)AssemblyMap::mov_imm_to_r8:
    {
        GetMovRmTOFR(readLen, assembly, mcs, eDirection::from, eWidth::eight);
        break;
    }
    }
    return readLen;
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
            ((assembly += " ") += reg1Str += ", ") += reg2Str += "\n";
        }
        else
        {
            ((assembly += " ") += reg2Str += ", ") += reg1Str += "\n";
        }
        
        readLen += 1;
        break;
    }
    case (unsigned char)eMod::disp_0:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
        string rmStr = GetRm(rm);

        if (dir == eDirection::to)
        {
            (((assembly += " ") += regStr += ", [") += rmStr += " + ") += "]\n";
        }
        else
        {
            ((assembly += " [") += rmStr += "], ") += regStr += "\n";
        }

        
        readLen += 1;
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
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]\n";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr += "\n";
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
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]\n";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr += "\n";
        }
        readLen += 3;
        break;
    }
    }
}


void GetMovImmToRm(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width)
{
    readLen += 1;
    assembly = "mov";
    unsigned char mod = GetBits(mcs[1], 0, 2);
    unsigned char rm = GetBits(mcs[1], 5, 3);
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
    switch (mod)
    {
    case (unsigned char)eMod::reg:
    {
        string regStr = width == eWidth::sixteen ? GetReg16(rm) : GetReg8(rm);
        (((assembly += " ") += regStr += ", ") += dispStr) += "\n";

        readLen += (width == eWidth::sixteen ? 2 : 1);
        break;
    }
    case (unsigned char)eMod::disp_0:
    {
        string rmStr = GetRm(rm);
        (((assembly += " [") += rmStr += "], ") += dispStr) += "\n";
        readLen += (width == eWidth::sixteen ? 2 : 1);
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


        (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr += "\n";


        readLen += (width == eWidth::sixteen ? 2 : 1);
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
            ((((assembly += " ") += regStr += ", [") += rmStr += " + ") += dispStr) += "]\n";
        }
        else
        {
            (((((assembly += " ") += "[") += rmStr += " + ") += dispStr) += "], ") += regStr += "\n";
        }
        readLen += (width == eWidth::sixteen ? 2 : 1);
        break;
    }
    }
}

int main()
{
    unsigned char mcs[10] = { 0x8B, 0x40, 0x8B, 0x40 };
    string assembly = "";
    int nLen = 0;
    int nCurrentPos = 0;
    while (true)
    {
        int restSize = 10 - nCurrentPos;
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
