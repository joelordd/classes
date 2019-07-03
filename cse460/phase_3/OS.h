/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 3
 * Date: 12/2/2018
 * File: OS.h
 * 
 * Declaration of the OS, inverted_pt_row and the PCB classes.  OS allows for process 
 * management by use of context switching.  PCB stores all the relevant info for each
 * process.
 * The OS class is a friend of the PCB class.
 * 
 * Declaration of the PT class allows each process to have a PageTable to 
 * keep track of which pages are allocated to which frame.
 * 
 * A new inverted page table class was created for the OS to keep track of which frames
 * were allocated to which pages. The pages were linked back to processes with PIDs.
 * PIDs are given out to processes based upon which ones were created first.
 * The degree of multiprogramming was implimented and newQ was created to hold all processes
 * that are waiting outside of the degree of multiprogramming.
 * freeFrame was created to keep track of which frames are free for use.
**/

#ifndef OS_H
#define OS_H

#include "VirtualMachine.h"
#include "Assembler.h"
#include "PT.h"

#include <fstream>
#include <list>
#include <queue>
#include <string>
#include <vector>


using namespace std;

class OS;

class PCB {
    public:
        PCB(const string &p, const int &b, const int &l, const int &page_size, const int &pid_num): 
            prog(p), base(b), limit(l), pc(b), sr(0), sp(256/*vm.msize*/),
            cpu_time(0), wait_time(0), io_time(0), wait_time_begin(0), 
            io_time_begin(0), ir(0), largest_stack_size(0), 
            turn_around_time(0), read_count(1), input_length(1),
            pid(pid_num), pt(page_size) { }
            
    private:
        int pc;
        int r[4];
        int ir, sr, sp, base, limit;
        int read_count;
        int input_length;
        int pid;
        
        string prog;
        fstream in, out;
        fstream stack;
        PT pt;
        
        int io_completion;
        int turn_around_time;
        int cpu_time;
        int wait_time, wait_time_begin;
        int io_time, io_time_begin;
        int largest_stack_size;
        
    friend
        class OS;
};

//This is a class for the inverted page table
class inverted_pt_row {
    private: 
        int pid;
        int page;
        
    friend
        class OS;
};

class OS {
    public:
        OS();
        ~OS();
        
        void run(string );
        void load_state();
        void save_state();
        void context_switch();
        void output_stats();
        void fifo(int );
        void lru(int );
        void loadPage(int );
        void dropFrame(int );
        
    private:
        VirtualMachine vm;
        Assembler as;
        
        //Used to count how many programs were loaded. Used for the deconstructor

        list<PCB *> pcb;        //jobs
        queue<PCB *> readyQ;
        queue<PCB *> waitQ;
        queue<PCB *> newQ;                  //Queue to keep waiting programs to be run outside of the 
                                            // programs currently running in the wait or ready or running queue
        PCB * running;
        
        //I think we should use a differnt data structure instead of avector 12.1.2018 - Peter
        
        queue<int> freeFrames;
        vector<inverted_pt_row> inv_pt;
        
        bool use_lru;                           //Indicator for page replacement algorithm
        bool use_fifo;
        int DoM;                            //Degree of Multiprogramming
        queue<int> pageInput;               //Tracks which pages comes first for fifo page replacement
        int stack_reserved; 
        const static int time_slice = 15;
        const static int context_switch_time = 5;
        fstream progs;
        int idle_time;
        int sys_time;  //total sys time        
        double program_count;               //program_count is used to count how many programs were ran.
    friend
        class VirtualMachine;
};

#endif