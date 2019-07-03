#!/bin/bash
touch log
g++ -c Assembler.cpp
printf "%s%d\n" "Assember: " "$?"
g++ -c VirtualMachine.cpp
printf "%s%d\n" "VirtualMachine: " "$?"
g++ -o os os.cpp Assembler.o VirtualMachine.o
printf "%s%d\n" "OS: " "$?"
./os sum1.s > log;
exit