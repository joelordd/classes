/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 2
 * Date: 10/22/2018
 * File: VirtualMachine.h
 * 
 * Declaration of the VirtualMachine class.  VM allows for time slicing and
 * mapping of logical addresses to physical addresses, along with keeping
 * track of the exit status of each process.
 * The OS class is a friend of this class.
**/


#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "OS.h"

#include <fstream>
#include <vector>

using namespace std;

class VirtualMachine {
    public:
        /*
            Constructor that initializes mem, clock, limit, registers, and 
            jumpOpcode, and array of function pointers.
        */
        VirtualMachine();
        void run();                                 //Runs the virtual machine.
        // int set_memory(const int&, const int&);     //Sets memory from OS.
        int setCarry(int);                          //sets carry bit in sr
        int signExtend(int&);                       //sign extends rd
        int setOverflow(int, int, int);             //sets overflow bit in sr
        int setStatus(int);                         //sets return status in sr
        int getCarry();                             //returns carry bit from sr

        /*
            Instruction functions, use jumpOpcode to jump to
            the instruction needed.
        */
        int load();
        int store();
        int add();
        int addc();
        int sub();
        int subc();
        int And();
        int Xor();
        int Compl();
        int shl();
        int shla();
        int shr();
        int shra();
        int compr();
        int getstat();
        int putstat();
        int jump();
        int jumpl();
        int jumpe();
        int jumpg();
        int call();
        int Return();
        int read();
        int write();
        int halt();
        int noop();
       
    private:
        int rsize;
        int msize;
        vector <int> r;
        vector <int> mem;
        int pc, ir, sr, sp, clock;
        int i, rd, rs, opcode, constant, addr;
        int base, limit;
        int total_limit;
        typedef int (VirtualMachine::*FP)();
        FP jumpOpcode[26];
        bool stop;                              //tells vm when to return to OS
    friend
        class OS;
};

#endif