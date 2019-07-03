#!/bin/bash
rm *.out
touch log
g++ -c Assembler.cpp
printf "%s%d\n" "Assember: " "$?"
g++ -c VirtualMachine.cpp
printf "%s%d\n" "VirtualMachine: " "$?"
g++ -o os os.cpp Assembler.o VirtualMachine.o
./os io.s > log
printf "%s%d\n" "OS: " "$?"
exit