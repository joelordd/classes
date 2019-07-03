#!/bin/bash
g++ -c Assembler.cpp
printf "%s%d\n" "Assember: " "$?"
g++ -c VirtualMachine.cpp
printf "%s%d\n" "VirtualMachine: " "$?"
g++ -c OS.cpp
printf "%s%d\n" "OS: " "$?"
g++ -o os os.cpp Assembler.o VirtualMachine.o
printf "%s%d\n" "OS: " "$?"
printf "Ready to Run\n"
exit