#include <stdio.h>

#include "disassembler.h"

int main()
{
    FILE* fp = fopen("logs.txt", "w");
    fclose(fp);

    struct disasm disasm = {};

    DisasmCtor(&disasm);

    DisassemblerMain(&disasm);

    DisassemblerMain(&disasm);

    return 0;
}
