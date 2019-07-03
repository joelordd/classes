#!/bin/bash

rm source_code/*.o
g++ -c source_code/Assembler.cpp
printf "%s%d\n" "Assember: " "$?"
g++ -c source_code/VirtualMachine.cpp
printf "%s%d\n" "VirtualMachine: " "$?"
g++ -c source_code/OS.cpp
printf "%s%d\n" "OS: " "$?"
g++ -o os source_code/os.cpp Assembler.o VirtualMachine.o OS.o
printf "%s%d\n" "OS: " "$?"
mv *.o source_code
printf "Ready to Run\n"
exit