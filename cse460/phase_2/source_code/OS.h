/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 2
 * Date: 10/22/2018
 * File: OS.h
 * 
 * Declaration of the OS and the PCB classes.  OS allows for process management
 * by use of context switching.  PCB stores all the relevant info for each
 * process.
 * The OS class is a friend of the PCB class.
**/

#ifndef OS_H
#define OS_H

#include "VirtualMachine.h"
#include "Assembler.h"

#include <fstream>
#include <list>
#include <queue>
#include <string>


using namespace std;

class OS;

class PCB {
    public:
        PCB(const string &p, const int &b, const int &l): 
            prog(p), base(b), limit(l), pc(b), sr(0), sp(256/*vm.msize*/),
            cpu_time(0), wait_time(0), io_time(0), wait_time_begin(0), 
            io_time_begin(0), ir(0), largest_stack_size(0), 
            turn_around_time(0), read_count(1), input_length(1) { }
    private:
        int pc;
        int r[4];
        int ir, sr, sp, base, limit;
        int read_count;
        int input_length;
        
        string prog;
        fstream in, out;
        fstream stack;
        
        
        int io_completion;
        int turn_around_time;
        int cpu_time;
        int wait_time, wait_time_begin;
        int io_time, io_time_begin;
        int largest_stack_size;
    friend
        class OS;
};

class OS {
    public:
        OS();
        ~OS();
        
        void run();
        void load_state();
        void save_state();
        void context_switch();
        void output_stats();
        
    private:
        VirtualMachine vm;
        Assembler as;
        list<PCB *> pcb; //jobs
        queue<PCB *> readyQ;
        queue<PCB *> waitQ;
        PCB * running;

        const static int time_slice = 15;
        const static int context_switch_time = 5;
        fstream progs;
        int idle_time;
        int sys_time;  //total sys time        
        //program_count is used to count how many programs were ran.
        double program_count;
};

#endif