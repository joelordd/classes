/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 1
 * Date: 10/14/2018
 * File: VirtualMachine.h
**/


#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <fstream>
#include <vector>

using namespace std;

class VirtualMachine {
    public:
        VirtualMachine();                           //Constructor that initializes mem, clock, limit, registers, and jumpOpcode, and array of function pointers
        void run(fstream&, fstream&);               //Runs the virtual machine.
        int get_clock() { return clock; }           //Returns amount of time program took to OS.
        int set_limit(const int&);                  //Sets limit from OS.
        int set_memory(const int&, const int&);     //Sets memory from OS.
        int setCarry(int);
        int signExtend(int&);
        int setOverflow(int, int, int);
        int getCarry();
        
        //Instruction functions, use jumpOpcode to jump to the instruction needed.
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
        fstream* input;
        fstream* output;
        int rsize;
        int msize;
        vector <int> r;
        vector <int> mem;
        int pc, ir, sr, sp, clock;
        int i, rd, rs, opcode, constant, addr;
        int base, limit;
        typedef int (VirtualMachine::*FP)();
        FP jumpOpcode[26];
        bool stop;
};

#endif