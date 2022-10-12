#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "stack.h"
#include "text-sort.h"

const int version = 1;

struct labelsinfo
{
    int labelnum;
    int labelbyte;
};

struct disasminfo
{
    int sign;
    int vers;
    int numofel;
};

struct disasm
{
    struct disasminfo info;
    struct labelsinfo labels[10];
    char* code;
    int ip;
};

enum ComArgs{
    ARG_MEM     = 128,
    ARG_REG     = 64,
    ARG_IMMED   = 32
};

enum DisassemblerErrors
{
    SignErr,
    VerErr,
};

int DisassemblerMain(struct disasm* disasm);

int DisasmCtor(struct disasm* disasm);

int DisasmDetor(struct disasm* disasm);

int DisasmCheck(struct disasm* disasm);

int GetArg(struct disasm* disasm, char cmd, char arg[]);

int GetReg(struct disasm* disasm, char reg[]);

#define DEF_CMD(name, num, arg, code) \
    CMD_##name = num,
enum Commands
{
#include "C:\Users\USER\Documents\GitHub\Assembler\cmd.h"
    NOCMD = 31
};
#undef DEF_CMD


#endif //DISASSEMBLER_H
