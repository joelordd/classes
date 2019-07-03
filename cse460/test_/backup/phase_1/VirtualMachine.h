#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <fstream>
#include <vector>

using namespace std;

/*
class exception {
    public:
        exception();
        nullPointerException();
        memNotAvailableException();
};*/

class VirtualMachine {
   public:
        VirtualMachine();                       //Constructor that initializes mem, clock, limit, registers, and jumpOpcode, and array of function pointers
        void run(fstream&, fstream&, fstream&); //Runs the virtual machine.
        int get_clock() { return clock; }       //Returns amount of time program took to OS.
        int set_limit(int&);                    //Sets limit from OS.
        int set_memory(int&);                   //Sets memory from OS.
        
        /*
            Instruction functions, use jumpOpcode to jump to the instruction needed.
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
        int base, limit;
       
    //https://stackoverflow.com/questions/5488608/how-define-an-array-of-function-pointers-in-c       
        typedef int (VirtualMachine::*FP);
        FP jumpOpcode[26];
};

#endif