#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

//Used for testing only:
#include <vector>

#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main(int argc, char *argv[])
{
    Assembler as;
    VirtualMachine vm;

    if (argc == 1) {
        cout << "Must supply an input file name.\n";
        exit(1);
    }
    string inputFile = argv[1];
    int pos = inputFile.find(".");
    if (pos > inputFile.length() || inputFile[pos+1] != 's' || inputFile[pos+2]) { //|| inputFile[pos+1] != 's' && !inputFile[pos+2]) {
        cout << "Invalid suffix! OS only uses '.s' suffixes only!\n";
        exit(2);
    }
    string name = inputFile.substr(0, pos);
    string outputFile = name + ".o";

    fstream assembly, objectCode;
    assembly.open(inputFile.c_str(), ios::in);
    objectCode.open(outputFile.c_str(), ios::out);

    if (as.assemble(assembly, objectCode)) {
        cout << "Assembler Error\n";
        assembly.close();
        objectCode.close();
        exit(3);
    }
    assembly.close();
    objectCode.close();
    objectCode.open(outputFile.c_str(), ios::in);
    
    fstream in, out;
    in.open((name + ".in").c_str(), ios::in);
    out.open((name + ".out").c_str(), ios::out);
    
    int instruction = 0, limit = 0;
    for (int i = 0; i < as.getlineCount(); i++) {
        objectCode >> instruction;
        vm.set_memory(instruction, i);
        limit++;
    }
    vm.set_limit(limit);
    vm.run(objectCode, in, out);
    out << "\nClock = " << vm.get_clock() << endl;
    
    objectCode.close();
  
    in.close();
    out.close();

    return 0;
} // main