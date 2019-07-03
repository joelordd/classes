/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 2
 * Date: 11/3/2018
 * File: OS.cpp
 * 
 * Creates an OS that enables process management through context switching on the virtual CPU.
 * PCBs are created to describe each process, and they are placed on a queue to determine priority.
 * There is a ready queue, which contains the processes ready to be ran, and a wait queue, which contains
 * the processes waiting for an i/o operation to complete.
**/

#include "OS.h"
#include "VirtualMachine.h"
#include "Assembler.h"

#include <fstream>
#include <iomanip>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <stdio.h>  //needed for remove()

using namespace std;

OS::OS () {
    idle_time = 0;
    sys_time = 0;
    program_count = 0;
}

        
OS::~OS() {
    list<PCB *>::iterator i;

    int cpu_time = 0;
    for (i = pcb.begin(); i != pcb.end(); i++)
        cpu_time += (*i)->cpu_time;
                
        //running->stack.open((running->prog + ".st").c_str(), ios::in);

    for (i = pcb.begin(); i != pcb.end(); i++) {
        //(*i)->in.open( ((*i)->prog + ".in").c_str(), ios::app);
        (*i)->out.open( ((*i)->prog + ".out").c_str(), ios::app);
        //(*i)->stack.open( ((*i)->prog + ".st").c_str(), ios::app);
        
        (*i)->out << "Turn around time = " << (*i)->turn_around_time << ", CPU time = " << (*i)->cpu_time
              << ", Wait time = " << (*i)->wait_time << ", IO time = " << (*i)->io_time << endl;

        (*i)->out << "Total CPU time = " << cpu_time << ", System time = " << sys_time
              << ", Idle time = " << idle_time << ", Final clock = " << vm.clock << endl
              << "Real CPU Utilization = " << setprecision(3) << (float) cpu_time / vm.clock * 100 << "%, "
              << "System CPU Utilization = " << (float) (vm.clock - idle_time) / vm.clock * 100 << "%, "
              << "Throughput = " << pcb.size() / ((float) vm.clock / 1000) << endl << endl;
        //(*i)->in.close();
        (*i)->out.close();
        //(*i)->stack.close();
    }
    // for (list<PCB*>::const_iterator it = this->pcb.begin(); it != this->pcb.end(); it++) {
    //     delete *it;
    // }
    // pcb.clear();
}

void OS::run() {
    system("ls *.s > progs");
    fstream assembly, objectCode;
    
    string progQueue = "progs";
    progs.open(progQueue.c_str(), ios::in);

    if (!progs.is_open()) {
        cout << "Couldn't open progs\n" << "Check if progs exists!\n";
        exit(1);
    }
    
    //string used just for getline
    string line = "";
     
    int memBase = 0, memLimit = 0; 
    
    //Loop assembles all programs in 'progs' file and puts them into memory.
    while(getline(progs, line)) {
        string program = "";
        istringstream str(line.c_str());
        str >> program;     //Stores the name of each program into a string
        
        //Checks if each program in 'progs' has right format
        int pos = program.find(".");
        if (pos > program.length() || program[pos+1] != 's' || program[pos+2]) { 
            cout << "Invalid suffix! OS only uses '.s' suffixes only!\n";
            exit(2);
        }
        
        string program_name = program.substr(0, pos);
        string output_code = program_name + ".o";
        
        //This is opens each program.s in progs.
        assembly.open(program.c_str(), ios::in);
        objectCode.open(output_code.c_str(), ios::out);        
        
        if ( !(assembly.is_open()) || !(objectCode.is_open())  ) {
            cout << "Error opening " << program << endl;
            exit(3);
        }
        //Assembles each program.
        if (as.assemble(assembly, objectCode)){        
            cout << "Assembler Error\n";
            assembly.close();
            objectCode.close();
            exit(5);
        }
        objectCode.close();
        
        objectCode.open(output_code.c_str(), ios::in);
        if (!objectCode.is_open()) {
            cout << "Couldn't open " << objectCode << endl;
            exit(5);
        }
        //This section loads instructions from the .o file into memory for VM
        for (int i = memBase; objectCode >> vm.mem[i]; i++, memLimit++);
        
        assembly.close();
        objectCode.close();  
        
        //This section creates a PCB for each process loaded into memory.
        PCB * p = new PCB (program_name, memBase, memLimit);
        
        //Creates program_name.st and .o files for each program.
        p->stack.open( (p -> prog + ".st").c_str(), ios::out);
        p->out.open( (p -> prog + ".out").c_str(), ios::out);
        p->stack.close();
        p->out.close();
        
        pcb.push_back(p);
        readyQ.push(p);
        memBase = memLimit;
        
        program_count++;
        

        //Counts the # of lines for input 
        //This is used for programs with multiple lines of input
        p->in.open((p->prog + ".in").c_str(), ios::in);
        if (!p->in.is_open()) {
            cout << "Couldn't open " << objectCode << endl;
            exit(5);
        }
        string count = "";
        while (getline(p->in, count)) { 
            string input_line = "";
            istringstream str(count.c_str());
            str >> input_line;
            (p->input_length)++;
        }
        p->in.close();
    }
    
    vm.total_limit = memLimit;
    progs.close();
    
    //Sets first program to as "running"
    running = readyQ.front();
    readyQ.pop();
    
    //Loads very first process from readyQ
    int time_stamp = 0;
    //Checks if running pointer is pointed to something
    while (running) {
        vm.clock += context_switch_time;
        sys_time += context_switch_time;
        load_state();

        time_stamp = vm.clock;
        vm.run();
        running->cpu_time += (vm.clock - time_stamp);
        context_switch();
    }
    

}

void OS::load_state(){
    vm.stop = false;
    if (!running) { 
        return;
    }
    vm.pc = running->pc;
    vm.sp = running->sp;
    vm.sr = running->sr;
    vm.r[3] = running->r[3]; 
    vm.r[2] = running->r[2]; 
    vm.r[1] = running->r[1]; 
    vm.r[0] = running->r[0];
    vm.ir = running->ir;
    
    vm.base = running->base;
    vm.limit = running->limit;

    //Loading stack only if it has something in it.
    if (running->sp < 256) {
        // cout << "\nLoading Stack\n";
        running->stack.open((running->prog + ".st").c_str(), ios::in);
        if (!( running->stack.is_open()) ) {
            cout << "Couldn't open " << running->prog << ".st for loading state" << endl;
            exit(6);
        }
        //Loading stack
        for (int i = 255; running->stack >> vm.mem[i]; i--);
        running->stack.close();

    } //stack is empty otherwise 
}

/*
Copies content from VM to the process's PCB.
*/
void OS::save_state() {    
    if (!running) { 
        cout << "Nothing running to save\n";
        return;
    }
    running->pc = vm.pc;
    running->sp = vm.sp;
    running->sr = vm.sr;    
    running->r[3] = vm.r[3];
    running->r[2] = vm.r[2];
    running->r[1] = vm.r[1];
    running->r[0] = vm.r[0];
    running->ir = vm.ir;
    
    running->base = vm.base;
    running->limit = vm.limit;

    //saving stack, only when stack has something in it
    if (running->sp < 256) {
        running -> stack.open((running->prog + ".st").c_str(), ios::out);
        
        if (!(running-> stack.is_open()) ) {
            cout << "Couldn't open " << running->prog << ".st for saving state" << endl;
            exit(7);
        }
        
        //Used for accounting information, calculating largest stack size.
        int stack_size = 256 - running -> sp;
        if (stack_size > running -> largest_stack_size) {
            running -> largest_stack_size = stack_size;
        }
        
        for (int i = 255; i >= running -> sp; i--) { 
            running->stack << vm.mem[i] << endl;
        }
        running->stack.close();
    }
    
}

void OS::context_switch() {
    while (not waitQ.empty() and waitQ.front()->io_completion <= vm.clock) {
        readyQ.push(waitQ.front());
        waitQ.front()->wait_time_begin = vm.clock;
        waitQ.front()->io_time += (vm.clock - waitQ.front()->io_time_begin);
        waitQ.pop();
    }
    
    int vm_status = vm.sr, overflow = vm_status;
    vm_status &= 0x0E0;    //gets rid of any bits that come before and after status bits;
    vm_status >>= 5;
    switch (vm_status) {
        case 0:         //Time slice
            readyQ.push(running);
            running->wait_time_begin = vm.clock;
            break;
        case 1: {        //Halt instruction
            cout << "Program " << running->prog << " has finished running.\n";
            running->out << running->prog << ": Terminated\n";
            running->turn_around_time = vm.clock;                
            
            // //deletes the .st file
            string rm = running->prog + ".st";
            remove(rm.c_str());
            break;
        }
        case 2:         //Out-of-Bound reference
            running->out << running->prog << ": Out of bound Error, pc = " << vm.pc << endl;
            running->turn_around_time = vm.clock;
            break;
        case 3:         //Stack Overflow  
            running->out << running->prog << ": Stack overflow, pc = " << vm.pc << ", sp = " << vm.sp << endl;
            running->turn_around_time = vm.clock;
            break;
        case 4:         //Stack underflow
            running->out << running->prog << ": Stack underflow, pc = " << vm.pc << ", sp = " << vm.sp << endl;
            running->turn_around_time = vm.clock;
            break;
        case 5:         //Invalid opcode
            running->out << running->prog << ": Bad opcode, pc = " << vm.pc << endl; 
            running->turn_around_time = vm.clock;
            break;
        case 6: {        //Read operation 
            int read =  vm.sr, reg;
            reg = read&0x300;
            reg >>= 8;
            if (reg < 0 || reg > 3) {
                printf("%s\n", "Invalid Register Value for read!");
                break;
            }
            running->in.open((running->prog + ".in").c_str(), ios::in);
            if (!(running -> in.is_open()) ) {
                cout <<  "Couldn't open " << running->prog << ".in to read from\n";
                exit(8);
            }
            
            for (int i = 0; i < running-> read_count; i++) {
                int temp = 0;
                running -> in >> temp;
                vm.r[reg] = temp;     
            }
            if (running->read_count <= running->input_length) running->read_count++;
            
            //Manually saving state;
            vm.sr &= 0x0FF;     //Resets I/O Register
            //Pushes program into waiting queue and does accounting
            waitQ.push(running);
            running->io_completion = vm.clock + 27;
            running->io_time_begin = vm.clock;
            running->in.close();
            break;
        }
        case 7: {        //Write instruction
            int write = vm.sr, reg;
            reg = write&0x300;

            reg >>= 8;
            
            if (reg < 0 || reg > 3) {
                printf("%s\n", "Invalid Register Value for write!");
                break;
            }
            //Using ofstream instead of fstream so it appends text instead
            ofstream ofile((running->prog + ".out").c_str(),  ios::app);
            if (!(ofile.is_open())) {
                cout <<  "Couldn't open " << running->prog << ".out to write to\n";
                exit(9);                    
            }
            ofile << vm.r[reg] << endl;        

            vm.sr &= 0x0FF;

            waitQ.push(running);
            running->io_completion = vm.clock + 27;
            running->io_time_begin = vm.clock;
            ofile.close();
            break;
        }
        default: 
            cerr << running->prog << ": Unexpected status = " << vm_status 
                 << " pc = " << vm.pc << " time = " << vm.clock << endl;
            running->out << running->prog << ": Unexpected status: " << vm_status 
                 << " pc = " << vm.pc << " time = " << vm.clock << endl;
            running->turn_around_time = vm.clock;
    }
    save_state();
    running = NULL;
    //Checks if readyQ is empty and all processes are in waiting queue doing I/O
    //Sets the clock to first process in waitQ to simulate waiting. 
    if (!(readyQ.empty())) {        
        running = readyQ.front();
        running->wait_time += (vm.clock - running->wait_time_begin);
        readyQ.pop();
    }
    else if (!waitQ.empty()) {
        running = waitQ.front();
        waitQ.pop();
        idle_time += (running->io_completion - vm.clock);
        vm.clock = running->io_completion;
        // assume all of context switch time is incurred after idle time
        running->io_time += (vm.clock - running->io_time_begin);     
    }
    //Third: Assigns next process from readyQ to VM (CPU). 
}