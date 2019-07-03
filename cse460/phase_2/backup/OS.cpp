/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 2
 * Date: 10/22/2018
 * File: OS.cpp
**/

#include <fstream>
#include <list>
#include <queue>
#include <string>
#include <iostream>
#include <cstdlib>
#include "OS.h"

using namespace std;

OS::OS () {
    
    
}

OS::~OS() {
    //destroy everything
    //free[all];
}
/*
    fact.s open.s test1.s => progs 
    
    
*/
void OS::run() {
    system("ls *.s > progs");
    fstream assembly, objectCode;
    string list = "progs";
    progs.open(list.c_str(), ios::in);

    //Loop assembles all programs in 'progs' file.
    programList<string>  = "";
    string programName = "";
    
    //Check for .s?
    for (int count = 0; getline(in, line); count++) {
        istringstream str(line.c_str());
        str >> programName;
        
        string outputFile = name;
        int pos = programName.find(".");
        programName = programName 
        
        programList.push(programName);
        assembly.open(programName[count].c_str(), ios::in);
        objectCode.open(outputFile.c_str(), ios::out);
        if (as.assemble(assembly, objectCode)){        
            cout << "Assembler Error\n";
            assembly.close();
            objectCode.close();
            exit(3);
        }
        assembly.close();
        objectCode.close();        
    }
    
    
    
    // //Puts instructions into memory for VM
    // int instruction = 0, limit = 0;
    // for (int i = 0; i < as.getlineCount(); i++, limit++) {
    //     objectCode >> instruction;
    //     if (vm.set_memory(instruction, i)) {
    //         cout << "Bad index...";
    //         return 1;
    //     }
    // }    
    // vm.set_limit(limit);
    // vm.run(in, out);
    // out << "\nClock = " << vm.get_clock() << endl;
    
    // objectCode.close();
  
    // in.close();
    // out.close();
}

void OS::load_state(){
    
}

void OS::save_state() {
    
}

void OS::context_switch() {
    
}
