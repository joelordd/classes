os: os.o OS.o VirtualMachine.o Assembler.o PT.o
	g++ -o os os.o OS.o VirtualMachine.o Assembler.o PT.o

os.o: os.cpp
	g++ -c os.cpp
    
OS.o: OS.cpp OS.h
	g++ -c OS.cpp

VirtualMachine.o: VirtualMachine.cpp VirtualMachine.h
	g++ -c VirtualMachine.cpp
    
Assembler.o: Assembler.cpp Assembler.h
	g++ -c Assembler.cpp
	
PT.o: PT.cpp PT.h
	g++ -c PT.cpp

clean:
	rm *.o os