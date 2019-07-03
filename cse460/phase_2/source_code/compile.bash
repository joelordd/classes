#!/bin/bash
# Description:  This script compiles the project for us. We also used it
#               for debugging purposes.

rm ./*.o
rm ./*.st
rm ./*.out

g++ -c ./Assembler.cpp
printf "%s%d\n" "Assembler: " "$?"
g++ -c ./VirtualMachine.cpp
printf "%s%d\n" "VirtualMachine: " "$?"
g++ -c ./OS.cpp
printf "%s%d\n" "OS: " "$?"
g++ -o os os.cpp Assembler.o VirtualMachine.o OS.o
printf "%s%d\n" "os: " "$?"
printf "Ready to Run\n"
#./os | head -n 200 > log
#./os
#For debugging segmentation faults
#http://www.cplusplus.com/articles/iwTbqMoL/
#
exit