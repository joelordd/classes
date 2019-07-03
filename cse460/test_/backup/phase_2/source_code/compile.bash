#!/bin/bash

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

#gdb ./os
printf "%s%d\n" "os: " "$?"
#mv *.o source_code
printf "Ready to Run\n"
./os | tail -n 500  > log
#./os
#For debugging segmentation faults
#http://www.cplusplus.com/articles/iwTbqMoL/
#
#
#g++ -go os os.cpp Assembler.o VirtualMachine.o OS.o

exit
