#include "pch.h"
#include "disasm_lib.h"
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
    mov_rm16_to_r16 = 0b10001011,
    mov_rm8_to_r8 = 0b10001010,
    mov_rm16_from_r16 = 0b10001001,
    mov_rm8_from_r8 = 0b10001000,
    mov_imm_to_rm16 = 0b11000111,
    mov_imm_to_rm8 = 0b11000110,
    mov_mem_to_acc16 = 0b10100001,
    mov_mem_from_acc16 = 0b10100011,
    mov_mem_to_acc8 = 0b10100000,
    mov_mem_from_acc8 = 0b10100010,
    mov_rm_to_seg = 0b10001110,
    mov_rm_from_seg = 0b10001100,
    mov_imm_to_r16 = 0b10111,
    mov_imm_to_r8 = 0b10110,
};


void GetMovRmTOFR(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width);
void GetMovImmToRm(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width);
void GetMovImmToR(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width);
void GetMovMemTOFAcc(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width);
void GetMovRmTOFSeg(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir);
string GetEAStr(int& readLen, unsigned char* dispBuf, unsigned char rm, unsigned char mod);
string GetDataStr(int& readLen, unsigned char* dataBuf, eWidth width);

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
    case (unsigned char)e16BReg::AX:
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
    default:
    {
        switch (mc >> 3)
        {
        case (unsigned char)AssemblyMap::mov_imm_to_r16:
        {
            GetMovImmToR(readLen, assembly, mcs, eWidth::sixteen);
            break;
        }
        case (unsigned char)AssemblyMap::mov_imm_to_r8:
        {
            GetMovImmToR(readLen, assembly, mcs, eWidth::eight);
            break;
        }
        }
        break;
    }
    }
    return readLen;
}

void GetMovMemTOFAcc(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width)
{
    readLen += 1;
    assembly = "mov";
    string regStr = width == eWidth::sixteen ? "ax" : "al";
    string eaStr = GetEAStr(readLen, &mcs[1], 0b110, 00);

    if (dir == eDirection::to)
    {
        (((assembly += " ") += regStr) += ", ") += eaStr;
    }
    else
    {
        (((assembly += " ") += eaStr) += ", ") += regStr;
    }
}

void GetMovRmTOFR(int& readLen, std::string& assembly, unsigned char* mcs, eDirection dir, eWidth width)
{
    readLen += 1;
    assembly = "mov";
    unsigned char mod = GetBits(mcs[1], 0, 2);
    unsigned char reg = GetBits(mcs[1], 2, 3);
    unsigned char rm = GetBits(mcs[1], 5, 3);
    string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
    switch (mod)
    {
    case (unsigned char)eMod::reg:
    {
        string reg2Str = width == eWidth::sixteen ? GetReg16(rm) : GetReg8(rm);
        if (dir == eDirection::to)
        {
            ((assembly += " ") += regStr += ", ") += reg2Str;
        }
        else
        {
            ((assembly += " ") += reg2Str += ", ") += regStr;
        }

        break;
    }
    default:
    {
        string eaStr = GetEAStr(readLen, &mcs[2], rm, mod);
        if (dir == eDirection::to)
        {
            ((assembly += " ") += regStr += ", ") += eaStr;
        }
        else
        {
            ((assembly += " ") += eaStr += ", ") += regStr;
        }
        break;
    }
    }
    readLen += 1;
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


        break;
    }
    default: {
        string eaStr = GetEAStr(readLen, &mcs[2], rm, mod);
        (assembly += " word ptr ") += eaStr;
        break;
    }
    }
    readLen += 1;
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
    {
        string dataStr = GetDataStr(readLen, &mcs[2], width);
        string regStr = width == eWidth::sixteen ? GetReg16(rm) : GetReg8(rm);
        (((assembly += " [") += regStr += "], ") += dataStr);

        break;
    }
    case (unsigned char)eMod::disp_0:
    {
        string eaStr = "";
        string dataStr = "";
        if (rm == 0b110)
        {
            eaStr = GetEAStr(readLen, &mcs[2], rm, mod);
            dataStr = GetDataStr(readLen, &mcs[4], width);
        }
        else
        {
            dataStr = GetDataStr(readLen, &mcs[2], width);
        }

        ((assembly += " ") += eaStr += ", ") += dataStr;
        break;
    }
    case (unsigned char)eMod::disp_low_extended:
    {

        string eaStr = GetEAStr(readLen, &mcs[2], rm, mod);
        string dataStr = GetDataStr(readLen, &mcs[3], width);

        ((assembly += " ") += eaStr += ", ") += dataStr;

        break;
    }
    case (unsigned char)eMod::disp_high_low:
    {
        string eaStr = GetEAStr(readLen, &mcs[2], rm, mod);
        string dataStr = GetDataStr(readLen, &mcs[4], width);

        ((assembly += " ") += eaStr += ", ") += dataStr;
        break;
    }
    }
    readLen += 1;
}

void GetMovImmToR(int& readLen, std::string& assembly, unsigned char* mcs, eWidth width)
{
    readLen += 1;
    assembly = width == eWidth::sixteen ? "mov word ptr" : "mov byte ptr";
    unsigned char reg = GetBits(mcs[0], 5, 3);
    string dataStr = GetDataStr(readLen, &mcs[1], width);

    assembly = "mov";
    string regStr = width == eWidth::sixteen ? GetReg16(reg) : GetReg8(reg);
    (((assembly += " ") += regStr += ", ") += dataStr);
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
    default:
    {
        string eaStr = GetEAStr(readLen, &mcs[2], rm, mod);
        if (dir == eDirection::to)
        {
            ((assembly += " ") += segStr += ", word ptr ") += eaStr;
        }
        else
        {
            ((assembly += " word ptr ") += eaStr += ", ") += segStr;
        }
        readLen += 1;
        break;
    }
    }
}

string GetEAStr(int& readLen, unsigned char* dispBuf, unsigned char rm, unsigned char mod)
{
    string result = "";
    if (mod == 1) {
        unsigned char* displow = dispBuf;
        char dispStr[5] = {};
        if (*displow > 0x79)
        {
            snprintf(dispStr, 5, "ff%02x", *displow);
        }
        else
        {
            snprintf(dispStr, 5, "00%02x", *displow);
        }
        string rmStr = GetRm(rm);
        readLen += 1;
        return ((((result += "[") += rmStr += " + ") += dispStr) += "]");
    }
    else
    {
        unsigned short* disp = (unsigned short*)dispBuf;
        char dispStr[5] = {};
        snprintf(dispStr, 5, "%04x", *disp);
        if (mod == 0 && rm == 0b110)
        {
            readLen += 2;
            return (((result += "[") += dispStr) += "]");
        }
        else if (mod == 0)
        {
            string rmStr = GetRm(rm);
            return ((result += "[") += rmStr += "]");
        }
        else
        {
            string rmStr = GetRm(rm);
            readLen += 2;
            return ((((result += "[") += rmStr += " + ") += dispStr) += "]");
        }
    }
}

string GetDataStr(int& readLen, unsigned char* dataBuf, eWidth width)
{
    unsigned char* datalow = dataBuf;
    unsigned short* data = (unsigned short*)dataBuf;
    char dataStr[5] = {};
    if (width == eWidth::sixteen)
    {
        snprintf(dataStr, 5, "%04x", *data);
        readLen += 2;
    }
    else
    {
        snprintf(dataStr, 5, "%02x", *datalow);
        readLen += 1;
    }

    return dataStr;
}