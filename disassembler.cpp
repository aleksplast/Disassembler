#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "disassembler.h"

static struct labelsinfo labels[10] = {0};

int DisassemblerMain(struct disasm* disasm)
{
    FILE* out = fopen("disasmout.txt", "w");
    int labelcounter = 1;

    for (disasm->ip = 0; disasm->ip < disasm->info.numofel; disasm->ip++)
    {
        char cmd = disasm->code[disasm->ip];
        int val1 = 0, val2 = 0;

        for (int i = 0; i < 10; i++)
        {
            if (disasm->ip == labels[i].labelbyte && labels[i].labelbyte != 0)
            {
                fprintf(out, "%d:\n", labels[i].labelnum);
                break;
            }
        }

        switch(cmd & 31)
        {
#define DEF_CMD(name, num, arg, cod)                                                        \
case CMD_##name:                                                                            \
    fprintf(out, "%s ", #name);                                                             \
    if (num <= 17 && num >= 10)                                                             \
    {                                                                                       \
        SetLabel(disasm, &labelcounter, out);                                               \
    }                                                                                       \
    else if  (arg == 1)                                                                     \
    {                                                                                       \
        char argm[30] = {};                                                                 \
        GetArg(disasm, cmd, argm);                                                          \
        fprintf(out, "%s", argm);                                                           \
    }                                                                                       \
    fprintf(out, "\n");                                                                     \
    break;
#include "..\Assembler\cmd.h"
#undef DEF_CMD
            default:
                printf("%c UNKNOWN COMMAND\n", *disasm->code);
                break;
        }
    }

    fclose(out);

    return NOERR;
}

int DisasmCtor(struct disasm* disasm)
{
    struct disasminfo info = {0, 0, 0};

    FILE* fp = fopen("out.txt", "r");

    long size = FileSize(fp);
    if ((disasm->code = (char*) calloc(size, sizeof(char))) == NULL)
        return MEMERR;

    fread(disasm->code, sizeof(char), size, fp);
    fclose(fp);

    DisasmCheck(disasm);

    return NOERR;
}

int DisasmDetor(struct disasm* disasm)
{
    free(disasm->code);
    disasm->ip = 0;

    return NOERR;
}

int DisasmCheck(struct disasm* disasm)
{
    disasm->info.sign = *(int*) disasm->code;
    if (disasm->info.sign != 'C' + 256 * 'P')
        return SignErr;

    disasm->info.vers = *(int*) (disasm->code + sizeof(int));
    if (disasm->info.vers != version)
        return VerErr;

    disasm->info.numofel = *(int*) (disasm->code + 2 * sizeof(int));

    disasm->code += 3 * sizeof(int);

    return NOERR;
}

int GetArg(struct disasm* disasm, char cmd, char arg[])
{
    if (cmd & ARG_MEM)
    {
        cmd = cmd - 128;
        char arg2[30] = {};
        GetArg(disasm, cmd, arg2);
        sprintf(arg, "[%s]", arg2);

        return NOERR;
    }
    if (cmd & ARG_IMMED)
    {
        sprintf(arg, "%lg", *(elem_t*)(disasm->code + disasm->ip + 1));
        disasm->ip += sizeof(elem_t);

        if (cmd & ARG_REG)
        {
            disasm->ip += sizeof(char);
            char reg[5] = {0};
            GetReg(disasm, reg);
            sprintf(arg + 1, "+%s", reg);
        }
    }
    else if (cmd & ARG_REG)
    {
        disasm->ip += sizeof(char);
        char reg[5] = {0};
        GetReg(disasm, reg);
        sprintf(arg, "%s", reg);
    }

    return NOERR;
}

int GetReg(struct disasm* disasm, char reg[])
{
    reg[0] = 'r';
    reg[2] = 'x';

    switch (disasm->code[disasm->ip])
    {
        case 1:
            reg[1] = 'a';
            break;
        case 2:
            reg[1] = 'b';
            break;
        case 3:
            reg[1] = 'c';
            break;
        case 4:
            reg[1] = 'd';
            break;
        default:
            printf("%d is an unknown register", *disasm->code);
            break;
    }

    return NOERR;
}

int SetLabel(struct disasm* disasm, int* labelcounter, FILE* out)
{
    bool flag = true;
    for (int i = 0; i < 10; i++)
    {
        if (*(int*)(disasm->code + disasm->ip + 1) == labels[i].labelbyte && labels[i].labelbyte != 0)
        {
            fprintf(out, ":%d", labels[i].labelnum);

            *labelcounter += 1;
            flag = false;
            break;
        }
    }
    if (flag)
    {
        fprintf(out, ":%d", *labelcounter);

        labels[*labelcounter].labelbyte = *(int*)(disasm->code + disasm->ip + 1);
        labels[*labelcounter].labelnum = *labelcounter;
    }

    *labelcounter += 1;
    disasm->ip += sizeof(int);
}
