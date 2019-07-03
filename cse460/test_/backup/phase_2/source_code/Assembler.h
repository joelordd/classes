/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 2
 * Date: 10/22/2018
 * File: Assembler.h
 * 
 * Declaration of the Assembler class. Assembler converts .s files into .o files
 * that the VM can understand.
**/
#ifndef ASSEMBLER_H
#define ASSEMBLER_H      

#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>

using namespace std;

//This class is meant to catch errors when a string is entered that is not a command
class NullPointerException: public runtime_error {
    public:
        NullPointerException(): runtime_error("Null Function Pointer!") { }
};

/**
 * This class is meant to take an assembly language program and convert it a single integer instruction that
 * the VM can read, outputting into an object file.
**/
class Assembler {
    public:
        Assembler();                            //Constructor that intialized the jumpTable
        int assemble(fstream&, fstream&);       //Main program, assembles the code into an object file readable by the VM.
        int getlineCount() { return lineCount; }        //Checks how many lines the object code is. Only the Assembler sets the lineCount
        
        //Instruction functions, use jumpTable to go to the correct function. 
        int load(istringstream &);
        int loadi(istringstream &);
        int store(istringstream &);
        int add(istringstream &);
        int addi(istringstream &);
        int addc(istringstream &);
        int addci(istringstream &);
        int sub(istringstream &);
        int subi(istringstream &);
        int subc(istringstream &);
        int subci(istringstream &);
        int And(istringstream &);
        int andi(istringstream &);
        int Xor(istringstream &);
        int xori(istringstream &);
        int Compl(istringstream &);
        int shl(istringstream &);
        int shla(istringstream &);
        int shr(istringstream &);
        int shra(istringstream &);
        int compr(istringstream &);
        int compri(istringstream &);
        int getstat(istringstream &);
        int putstat(istringstream &);
        int jump(istringstream &);
        int jumpl(istringstream &);
        int jumpe(istringstream &);
        int jumpg(istringstream &);
        int call(istringstream &);
        int Return(istringstream &);
        int read(istringstream &);
        int write(istringstream &);
        int halt(istringstream &);
        int noop(istringstream &);
            
    private:
        int lineCount;
        typedef int (Assembler::*FP)(istringstream &);
        map<string, FP> jumpTable;
};
#endif