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
#include <list>
#include <queue>
#include <string>
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
    for (list<PCB*>::const_iterator it = this->pcb.begin(); it != this->pcb.end(); it++) {
        delete *it;
    }
    pcb.clear();
}

void OS::run() {
    system("ls *.s > progs");
    //system("ls test_programs/*.s > progs");
    fstream assembly, objectCode;
    
    //I had to make a temp string variable for it to work on c_str().
    //c_str doesn't take direct strings: "progs".c_str()
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
        if (pos > program.length() || program[pos+1] != 's' || program[pos+2]) { //|| inputFile[pos+1] != 's' && !inputFile[pos+2]) {
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
        
        //Notice the ios::in and ios::out
        //Reopens {program}.o as input to our program.
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
    
    progs.close();
    
    //Sets first program to as "running"
    running = readyQ.front();
    readyQ.pop();
    //Loads very first process from readyQ
    load_state();
    
    //Checks if running pointer is pointed to something
    while (running) {
        cout << "\nNow running: " << running -> prog << endl;
        vm.run();
        save_state();
        context_switch();
        load_state();
        
    }
    cout << "Floating point test1\n";
    int final = (sys_time / 1000);
    int util = ((sys_time - idle_time) / sys_time) * 100;
    int thru = ( 6 / (sys_time / 1000));
    cout << "Floating point test2\n";
    //output sys_time here in seconds
    printf ("%s%d%s%d%s\n", "System time: ", sys_time, " ticks, ", final, " seconds");
    
    //This is process specific, need to print in hte .out file instead of on the OS ending
    //printf ("%s%10d\n", "Largest stack size: ", largest_stack_size);
    printf ("%20s%10d\n", "Idle Time: ", idle_time);
    printf ("%20s%10d%s\n", "CPU Utilization: ", util, "%");
    printf ("%20s%10d%s\n", "Throughput: ", thru, " procs/sec");
    
}

void OS::load_state(){
    vm.stop = false;
    if (!running) { 
        cout << "Cannot load state when nothing is running...\n";
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
        
        // //Resets stack file
        // running->stack.open((running->prog + ".st").c_str(), ios::out | ios::trunc);
        // running->stack.close();
    } //stack is empty otherwise 
}

/*
Copies content from VM to the process's PCB.
*/
void OS::save_state() {
    // cout << "++++++++++++++++++++++++++++++" << endl;
    // cout << "Saving state for the program, " << running -> prog << endl;
    // cout << "Program's pc is " << vm.pc << endl;
    // cout << "Program's sp is " << vm.sp << endl;
    // for (int i = 0; i<4; i++) cout << "r[" << i << "] = " << vm.r[i] << endl;
    // cout << "++++++++++++++++++++++++++++++\n" << endl;
    
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


    //Turnaround Time: time up to and including the halt instruction execution. 
    running->turn_around_time += vm.clock;
    
    vm.clock = 0;
    
    //saving stack, only when stack has something in it
    if (running->sp < 256) {
        // cout << "Saving stack for the program, " << running -> prog << endl;
        // cout << "Program's sp is " << vm.sp << endl;
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
            // cout << vm.mem[i] << endl;
        }
        running->stack.close();
    }
    
}


//Added parameters for this function
void OS::context_switch() {
    
    //first, all processes in waitQ whose I/O operation has been completed are placed in readyQ,
    
    //Checks I/O completion time of front of waiting queue.
    //Checks waitQ to see if processes can be moved to readyQ
    //cout << "Starting context switch\n";
    if (!waitQ.empty()) {
        while (waitQ.size() > 0){
            //Would set I/O time here.
            if (waitQ.front()->io_completion <= sys_time) {
            waitQ.front()->io_time += (sys_time - waitQ.front()->io_time_begin);
            readyQ.push(waitQ.front());
            waitQ.pop();
            } else break;
        }
    }
    //second the running process is placed in the proper queue or terminated
    int status = vm.sr, overflow = status;
        status &= 0x0E0;    //gets rid of any bits that come before and after status bits;
        status >>= 5;
        cout << "\nNow context switching with status: " << status << endl;
        switch (status) {
            case 0:         //Time slice
                cout << "Time Sliced...\n";
                readyQ.push(running);
                break;
            case 1: {        //Halt instruction
                cout << "Program " << running->prog << " has finished running.\n";
                //deletes the .st file
                string rm = running->prog + ".st";
                remove(rm.c_str());
                running = NULL;
                break;
            }
            case 2:         //Out-of-Bound reference
                printf("%s\n", "Out-of-Bound Reference, trying to access unallocated memory!");
                running = NULL;
                break;
            case 3:         //Stack Overflow  
                printf ("%s\n", "Stack Overflow, not enough memory available!");
                running = NULL;
                break;
            case 4:         //Stack underflow
                printf ("%s\n", "Stack Underflow, stack is empty!");
                running = NULL;
                break;
            case 5:         //Invalid opcode
                printf ("%s\n", "Invalid Opcode!");
                running = NULL;
                break;
            case 6: {   //Performs the actual read operation
                cout << "READING.....\n";
                int read = running -> sr, reg;
                reg = read&0x300;
                reg >>= 8;
                if (reg < 0 || reg > 3) {
                    printf("%s\n", "Invalid Register Value for read!");
                    break;
                }
                cout << "Input_length: " << running->input_length << endl;       //Read operation
                running->in.open((running->prog + ".in").c_str(), ios::in);
                if (!(running -> in.is_open()) ) {
                    cout <<  "Couldn't open " << running->prog << ".in to read from\n";
                    exit(8);
                }
                
                int temp = 0;
                if (running -> read_count == running->input_length) {
                    cout << "If triggered!\n";
                    running-> in >> temp;
                    vm.r[reg] = temp;
                } else {
                    for (int i = 0; i < running-> read_count; i++) {
                        int temp = 0;
                        running -> in >> temp;
                        cout << "TEMP:" << temp << endl;
                        vm.r[reg] = temp;                    
                    }
                    if (running->read_count <= running->input_length) running->read_count++;
                }
                //running->in >> vm.r[reg];
                
                cout << "\nOS:\n Finished accessing r[" << reg << "] = " << vm.r[reg] << endl;
                //Manually saving state;
                vm.sr &= 0x0FF;     //Resets I/O Register
                save_state();
                //Pushes program into waiting queue and does accounting
                waitQ.push(running);
                running->io_time_begin = sys_time;
                running->io_completion = sys_time + 27;
                running->in.close();
                break;
            }
            case 7: {        //Write instruction
                int write = running -> sr, reg;
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
                cout << "\nWriting to r[" << reg << "] value: " << vm.r[reg] << endl;
                
                waitQ.push(running);
                running->io_time_begin = sys_time;
                running->io_completion = sys_time + 27;
                //running->out.close();
                ofile.close();
                break;
            }
            default: 
                printf ("%s\n", "Unrecognized code!");
                exit(10);
        }
    //Checks if readyQ is empty and all processes are in waiting queue doing I/O
    //Sets the clock to first process in waitQ to simulate waiting. 
    if (readyQ.empty() && !waitQ.empty()) {
        //Matches completion time of earliest I/O request.
        waitQ.front()->wait_time = waitQ.front()->io_completion - sys_time;
        idle_time += waitQ.front()->wait_time;
        sys_time += idle_time;
        
        readyQ.push(waitQ.front());
        waitQ.pop();
    }
    //Third: Assigns next process from readyQ to VM (CPU). 
    if (!(readyQ.empty())) {
        running = readyQ.front();
        readyQ.pop();
    }
    sys_time += 5;
}