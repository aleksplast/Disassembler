CC := g++ -Wno-pointer-arith
DIRB := C:\Users\USER\Documents\GitHub\Disassembler
DIRC := C:\Users\USER\Documents\GitHub\Disassembler
SRC := $(DIRC)\main.cpp $(DIRC)\disassembler.cpp $(DIRC)\stack.cpp $(DIRC)\text-sort.cpp

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIRC)\$@

run: $(DIR)\main.exe
	$(DIR)\main.exe

