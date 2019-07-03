/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 1
 * Date: 10/14/2018
 * File: os.cpp
**/


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
    string assemblyFile = argv[1];
    int pos = assemblyFile.find(".");
    if (pos > assemblyFile.length() || assemblyFile[pos+1] != 's' || assemblyFile[pos+2]) { //|| assemblyFile[pos+1] != 's' && !assemblyFile[pos+2]) {
        cout << "Invalid suffix! OS only uses '.s' suffixes only!\n";
        exit(2);
    }
    string name = assemblyFile.substr(0, pos);
    string objectFile = name + ".o";

    fstream assembly, objectCode;
    assembly.open(assemblyFile.c_str(), ios::in);
    objectCode.open(objectFile.c_str(), ios::out);
    if (!assembly.is_open() or !objectCode.is_open()) {
        cout << "Couldn't open " << assemblyFile << " and/or " << objectFile << endl;
        exit(3);
    }
    if (as.assemble(assembly, objectCode)) {
        cout << "Assembler Error\n";
        assembly.close();
        objectCode.close();
        exit(4);
    }
    assembly.close();
    objectCode.close();
    
    
    
    objectCode.open(objectFile.c_str(), ios::in);
    if (!objectCode.is_open()) {
        cout << "Couldn't open " << objectFile << endl;
        exit(5);
    }    
    string inFile = name + ".in";
    string outFile = name + ".out";    
    
    fstream in, out;
    in.open((name + ".in").c_str(), ios::in);
    out.open((name + ".out").c_str(), ios::out);
    if (!in.is_open() or !out.is_open()) {
        cout << "Couldn't open " << inFile << " and/or " << outFile << endl;
        exit(6);
    }
    //Puts instructions into memory for VM
    int instruction = 0, limit = 0;
    for (int i = 0; i < as.getlineCount(); i++, limit++) {
        objectCode >> instruction;
        if (vm.set_memory(instruction, i)) {
            cout << "Bad index... Exiting VM";
            return 1;
        }
    }
    vm.set_limit(limit);
    
    vm.run(in, out);
    out << "\nClock = " << vm.get_clock() << endl;
    
    objectCode.close();
    in.close();
    out.close();
    
    return 0;
} // main