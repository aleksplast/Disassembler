#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "..\Assembler\Common.h"
#include "stack.h"
#include "text-sort.h"

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
    char* code;
    int ip;
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

int SetLabel(struct disasm* disasm, int* labelcounter, FILE* out);

#endif //DISASSEMBLER_H
