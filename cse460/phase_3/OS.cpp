/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 3
 * Date: 12/2/2018
 * File: OS.cpp
 * 
 * Creates an OS that enables process management through context switching on the virtual CPU.
 * PCBs are created to describe each process, and they are placed on a queue to determine priority.
 * There is a ready queue, which contains the processes ready to be ran, and a wait queue, which contains
 * the processes waiting for an i/o operation to complete.
 * 
 * Phase 3 Update:
 * In the context switch, the OS detects if the VM returned because of a page fault. 
 * New functions were created (LRU/FIFO) to handle page replacement if there are no
 * available frames left.
**/

#include "OS.h"
#include "VirtualMachine.h"
#include "Assembler.h"
#include "PT.h"

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
    use_lru = false;
    use_fifo = false;
    //Reserving 3 frames for the stack, so stack_reserved = 3 frames * 8 = 24
    stack_reserved = 24;
    inv_pt.reserve(32);
    //Only 0-28 because frames 31-29 are reserved for the stack.
    for(int i = 0; i < 29; i++) freeFrames.push(i);       
    //the 32nd frame will be reserved for the stack.
    DoM = 5;
}

        
OS::~OS() {
    list<PCB *>::iterator i;

    int cpu_time = 0;
    for (i = pcb.begin(); i != pcb.end(); i++)
        cpu_time += (*i)->cpu_time;
                
    for (i = pcb.begin(); i != pcb.end(); i++) {
        (*i)->out.open( ((*i)->prog + ".out").c_str(), ios::app);
        
        (*i)->out << "Turn around time = " << (*i)->turn_around_time << ", CPU time = " << (*i)->cpu_time
              << ", Wait time = " << (*i)->wait_time << ", IO time = " << (*i)->io_time << endl;

        (*i)->out << "Total CPU time = " << cpu_time << ", System time = " << sys_time
              << ", Idle time = " << idle_time << ", Final clock = " << vm.clock << endl
              << "Real CPU Utilization = " << setprecision(3) << (float) cpu_time / vm.clock * 100 << "%, "
              << "System CPU Utilization = " << (float) (vm.clock - idle_time) / vm.clock * 100 << "%, "
              << "Throughput = " << pcb.size() / ((float) vm.clock / 1000) << endl << endl;
        (*i)->out.close();
    }
    // for (list<PCB*>::const_iterator it = this->pcb.begin(); it != this->pcb.end(); it++) {
    //     delete *it;
    // }
}

void OS::run(string alg) {
    
    if(alg == "-lru") use_lru = true;       //this is for indicating lru page replacement algorithm
    else if (alg == "-fifo") use_fifo = true;
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
     
    int assignPID = 0, memBase = 0, frameNumber; 
    
    //Loop assembles all programs in 'progs' file and puts them into memory.
    while(getline(progs, line)) {
        int memLimit = 0;
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
            exit(4);
        }
        objectCode.close();
        
        //This section counts how many lines of code are in a program.
        //This will later be used to divide the program into pages.
        std::ifstream code (output_code.c_str(), std::ifstream::binary);
        if (code) {
            // get length of file:
            code.seekg (0, code.end);
            memLimit = code.tellg();
            code.seekg (0, code.beg);
            
            //6 = 6 bytes per line, divide by 6 to get number of lines
            memLimit /= 6; 
        }
        code.close();

        //This section is used to load a single page into the VM.
        objectCode.open(output_code.c_str(), ios::in);
        if (!objectCode.is_open()) {
            cout << "Run1: Couldn't open " << objectCode << endl;
            exit(5);
        }
        //Loads only one page from the program.
        int page_length = memBase + 7;
        for (int i = memBase; memBase <= page_length; memBase++, i++) {
            objectCode >> vm.mem[memBase];  
        }
        //Pushes first page into FIFO queue.    
        if (use_fifo) pageInput.push(frameNumber);
            
        //Pops Frame X to note that its 'used' since we're going in numerical order.
        freeFrames.pop();
        
        assembly.close();
        objectCode.close();  
        
        int page_count = memLimit / 8;                   //calculates how many pages there are for the program.
        if((memLimit % 8) != 0) page_count += 1;         //adds another page if there is internal fragmentation
        
        //This section creates a PCB for each process loaded into memory.
        PCB * p = new PCB (program_name, 0, memLimit, page_count, assignPID);
        p->pt.page_table[0].frame = 0;
        p->pt.page_table[0].valid = true;
        
        //Counts 0 to 4, which is 5 total for the DOM
        if (program_count < DoM) {
            readyQ.push(p);       //pushes process to ready if less than 5 processes already in it
            inv_pt[assignPID].pid = assignPID;
            inv_pt[assignPID].page = 0;
        }
        else { //else, program is not running.
            newQ.push(p);                             
        }
        program_count++;
        assignPID++;        //this is for assigning each process an ID.
        
        //Creates program_name.st and .o files for each program.
        p->stack.open( (p -> prog + ".st").c_str(), ios::out);
        p->out.open( (p -> prog + ".out").c_str(), ios::out);
        p->stack.close();
        p->out.close();
        
        pcb.push_back(p);

        //Counts the # of lines for input 
        //This is used for programs with multiple lines of input
        cout << "p name" << p->prog << endl; 
        p->in.open(program.c_str(), ios::in);
        if (!p->in.is_open()) {
            cout << "Run2: Couldn't open " << objectCode << endl;
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
    if (!running) { 
        return;
    }
    vm.stop = false;

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
    vm.tlb = running->pt;       //copies page table to the tlb

    //Loading stack only if it has something in it.
    if (running->sp < 256) {
            
        int stack_size = 255 - (running -> sp);
        if (stack_size > stack_reserved) {
            stack_reserved = stack_size;
            
            //Calculate if an extra page is needed
            int pages_used = stack_size / 8;
            int extra_page = stack_size % 8;
            
            switch (extra_page) {
                case 6:     //No extra page needed
                    break;
                case 4:     //Stack needs a portion of next page
                    pages_used++;
                    break;
                case 2:     //Stack needs a portion of next page
                    pages_used++;
                    break;
                case 0:     //No extra page needed
                    break;
                default: { 
                    cout << "OS: Bad stack size!\n";
                    exit(3);
                }
            }
            //Kicking out pages used
            for (int i = 31; i >= (31 - pages_used); i--) {
                int victim = i;
                //update inverted page table
                inv_pt[victim].pid = running->pid;
                //Using 1000 as a signal that it is being used a stack
                inv_pt[victim].page = 1000;
                
                //Update Victim process's Page table
                list<PCB *>::iterator j;
                for (j = pcb.begin(); j != pcb.end(); j++) {
                    if((*j)->pid == inv_pt[victim].pid) {
                        (*j)->pt.page_table[victim].valid = false;
                        break;
                    } 
                }            
            }
        }
        running->stack.open((running->prog + ".st").c_str(), ios::in);
        if (!( running->stack.is_open()) ) {
            cout << "load_state function: Couldn't open " << running->prog << ".st\n";
            exit(6);
        }
        for (int i = 255; running->stack >> vm.mem[i]; i--);
        running->stack.close();

    }
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
            cout << "save_state: Couldn't open " << running->prog << ".st for saving state" << endl;
            exit(7);
        }
        
        //Used for accounting information, calculating largest stack size.
        int stack_size = 256 - running -> sp;
        
        //Variable for keeping track of how much memory is used for the stack
        if (stack_reserved < stack_size)
            stack_reserved = stack_size; 
        
        if (stack_size > running -> largest_stack_size) 
            running -> largest_stack_size = stack_size;
        for (int i = 255; i >= running -> sp; i--) { 
            running->stack << vm.mem[i] << endl;
        }
        running->stack.close();
    }
    
}

void OS::context_switch() {
    //cout << "Context switch\n";
    while (not waitQ.empty() and waitQ.front()->io_completion <= vm.clock) {
        readyQ.push(waitQ.front());
        waitQ.front()->wait_time_begin = vm.clock;
        waitQ.front()->io_time += (vm.clock - waitQ.front()->io_time_begin);
        waitQ.pop();
    }
    
    int vm_status = vm.sr, check_PF = vm_status;
    vm_status &= 0x0E0;    //gets rid of any bits that come before and after status bits;
    vm_status >>= 5;
    switch (vm_status) {
        case 0: {        //Time slice
            //1 00 000 00000
            check_PF &= 0x400;
            check_PF >>= 10;
            
            if (check_PF == 1) {  //Page Fault
                int page = vm.sr & 0xFC00;
                page >>= 11;
                
                //Checks if page in SR is incorrectly set
                if (page > running->pt.ptSize) {
                    cout << "\nInvalid page!\n";
                    exit(3);
                }
                //we are using an int variable that lists how many pages are free so our program doesn't loop so much!
                if (freeFrames.size() > 0) {
                    cout << "Checking for free frames...\n";
                    int victim_frame = freeFrames.front();
                    running -> pt.page_table[page].valid = true;
                    running -> pt.page_table[page].frame = victim_frame;
                    
                    inv_pt[victim_frame].pid = running -> pid;
                    inv_pt[victim_frame].page = page;
                    freeFrames.pop();
                    
                } else {                    
                    cout << "No more free frames...\n";

                    //if there are still no free frames, do the following:
                    if (use_lru) {
                        lru(page);
                    }
                    else if (use_fifo) {
                        fifo(page);
                    }
                    else 
                        cout << "Context Switch: No valid method!\n";
                        exit(4);
                }
                loadPage(page);
                waitQ.push(running);
                running->io_completion = vm.clock + 27;
                running->io_time_begin = vm.clock;
                
                //sets page_times for lru
                vm.page_times[running->pt.page_table[page].frame] = vm.clock;
            
                vm.sr &= 0x01F;     //Resets SR and saves statuses only, no PAGE/IO/etc
            }
            else {  //Time Slice
                readyQ.push(running);
                running->wait_time_begin = vm.clock;
                vm.sr &= 0x01F;     //Resets SR and saves statuses only
            }
            break;
        }
        case 1: {        //Halt instruction
            cout << "Program " << running->prog << " has finished running.\n";
            running->out << running->prog << ": Terminated\n";
            running->turn_around_time = vm.clock;                
            
            //deletes the .st file
            string rm = running->prog + ".st";
            remove(rm.c_str());
            
            /*
            Go into running process's Page table
            Free the frames where the valid bit is set
            */
            for (int i = 0; i < running->pt.ptSize; i++) {
                if (running->pt.page_table[i].valid == true) {
                    freeFrames.push(i);
                    running->pt.page_table[i].valid = false;
                }
            }
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
            cout << "read\n";
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
            
            vm.sr &= 0x01F;     //Resets I/O Register and saves statuses only
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

            //          P#  PF IO VMR Status
            //         00000 0 00 000 11111
            vm.sr &= 0x01F;     //Resets I/O Register and saves statuses only
            
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
    
    //adds processes to readyQ if there are less processes than the degree of multiprogramming
    if( (readyQ.size() + waitQ.size()) <= DoM && !newQ.empty()) {
        //Progs seeksg
        readyQ.push(newQ.front());
        newQ.pop();
    }
}

//
void OS::fifo(int new_page) {
    //this is the frame that will be kicked out
    int victim = pageInput.front();
    
    //assign the victim frame to page needed
    running->pt.page_table[new_page].valid = true;
    running->pt.page_table[new_page].frame = victim;
    
    //set the valid bit of the victim process to false
    list<PCB *>::iterator i;
    for (i = pcb.begin(); i != pcb.end(); i++) {
        if((*i)->pid == inv_pt[victim].pid) {
            (*i)->pt.page_table[new_page].valid = false;
            break;
        } 
    }
    
    //update inverted page table
    inv_pt[victim].pid = running->pid;
    inv_pt[victim].page = new_page;
    
    pageInput.pop();
}

void OS::lru(int new_page) {
    int victim = 0;
    
    
    //finds the page with the smallest(least used) time
    for(int i = 0; i < 31; i++) {
        if(vm.page_times[i] < vm.page_times[i+1] && vm.page_times[i] != 0) victim = i;
    }
    
    //replaces the page
    running->pt.page_table[new_page].valid = true;
    running->pt.page_table[new_page].frame = victim;
    
    //set the valid bit of the victim process to false
    list<PCB *>::iterator i;
    for (i = pcb.begin(); i != pcb.end(); i++) {
        if((*i)->pid == inv_pt[victim].pid) {
            (*i)->pt.page_table[new_page].valid = false;
            break;
        } 
    }
    
    //update inverted page table
    inv_pt[victim].pid = running->pid;
    inv_pt[victim].page = new_page;
    
    //set that pageTime entry to the current clock
    vm.page_times[victim] = vm.clock;
}

void OS::loadPage(int page) {
    fstream objectCode;
    string obfile = running->prog + ".o";
    
    objectCode.open(obfile.c_str(), ios::in);
    if (!objectCode.is_open()) {
        cout << "loadpage function: Couldn't open " << objectCode << endl;
        exit(5);
    }
    int Memstart = running->pt.page_table[page].frame * 8;     //starting point for virtual memory
    int VMstart = page * (6*8);                     //6 = 5 characters of the object + \n. starting point for object code
    objectCode.seekg(VMstart, ios::beg);            //points to the start of the correct page in the object code(disk)
    
    for(int i = 0; i < 8; i++, Memstart++) {
        objectCode >> vm.mem[Memstart];
        cout << "MEMSTART: VM[" << Memstart  << "] " << endl;
        cout << "VM VALUE: " << vm.mem[Memstart] << endl;
    }
    
    objectCode.close();
}

void OS::dropFrame (int frame) {
    int victim_pid = inv_pt[frame].pid;

    //Update Victim process's Page table
    list<PCB *>::iterator j;
    for (j = pcb.begin(); j != pcb.end(); j++) {
        if( (*j)->pid == inv_pt[victim_pid].pid) {
            (*j)->pt.page_table[frame].valid = false;
            break;
        } 
    }            
    //update inverted page table
    inv_pt[victim_pid].pid = running->pid;
    //Using 1000 as a signal that it is being used a stack
    inv_pt[victim_pid].page = 1000;
    
}
