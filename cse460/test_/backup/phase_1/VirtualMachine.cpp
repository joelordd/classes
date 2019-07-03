#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>

#include "VirtualMachine.h"

using namespace std;

VirtualMachine::VirtualMachine () {
    msize = 256;
    rsize = 4;
    clock = 0;
    
    mem.reserve(msize); 
    r.reserve(rsize); 
    jumpOpcode[0]  = &VirtualMachine::load;
    jumpOpcode[1]  = &VirtualMachine::store;
    jumpOpcode[2]  = &VirtualMachine::add;
    jumpOpcode[3]  = &VirtualMachine::addc;
    jumpOpcode[4]  = &VirtualMachine::sub;
    jumpOpcode[5]  = &VirtualMachine::subc;
    jumpOpcode[6]  = &VirtualMachine::And;
    jumpOpcode[7]  = &VirtualMachine::Xor;
    jumpOpcode[8]  = &VirtualMachine::Compl;
    jumpOpcode[9]  = &VirtualMachine::shl;
    jumpOpcode[10] = &VirtualMachine::shla;
    jumpOpcode[11] = &VirtualMachine::shr;
    jumpOpcode[12] = &VirtualMachine::shra;
    jumpOpcode[13] = &VirtualMachine::compr;
    jumpOpcode[14] = &VirtualMachine::getstat;
    jumpOpcode[15] = &VirtualMachine::putstat;
    jumpOpcode[16] = &VirtualMachine::jump;
    jumpOpcode[17] = &VirtualMachine::jumpl;
    jumpOpcode[18] = &VirtualMachine::jumpe;
    jumpOpcode[19] = &VirtualMachine::jumpg;
    jumpOpcode[20] = &VirtualMachine::call;
    jumpOpcode[21] = &VirtualMachine::Return;
    jumpOpcode[22] = &VirtualMachine::read;
    jumpOpcode[23] = &VirtualMachine::write;
    jumpOpcode[24] = &VirtualMachine::halt;
    jumpOpcode[25] = &VirtualMachine::noop;
    
}

int VirtualMachine::set_limit(int &x) {
    const int success = false;
    const int error = true;
    if (x < 0 || x > 256) {
        return error;
    } else {  
        limit=x;
        return success;
    }
}

int VirtualMachine::set_memory(int& instruction, x) {
    const int success = false;
    const int error = true;
    if (x < 0 || x > 256) {
        return error;
    } else {
        mem[x] = instruction;
        return success;
    }
}

void VirtualMachine::run (fstream& objectCode, fstream& in, fstream& out)
{
    const int debug = false;
    int opcode, rd, i, rs, constant, addr, j;
    
    base = 0;
    //Need to move this to OS 
    //for (limit = 0; objectCode >> mem[limit]; limit++);
    
    sr = 2;
    sp = msize;
    pc = 0;
    
    while (pc < limit) {
        ir = mem[pc];
        pc++;
        //0xf800 = 1111,1000,0000,0000 [0,15] 16 bits 
        opcode = (ir&0xf800)>>11;
        //0x600 = 110,0000,0000 11 [0,10] bits 
        rd = (ir&0x600)>>9;
        //0x100 = 1,0000,0000 [0,6] 7 bits total
        i = (ir&0x100)>>8;
        //0xc0 = 1100,0000 [0,7 bits], 8 bit total
        rs = (ir&0xc0)>>6;
        
        addr=ir&0xff;
        constant = addr;
        
        if (ir&0x80) constant |= 0xffffff00; // if neg sign extend
        
        clock++;
        
        this->*jumpOpcode[opcode];
        
    }
    if (debug) { 
        printf("ir=%d op=%d rd=%d i=%d rs=%d const=%d addr=%d\n", ir, opcode, rd, i, rs, constant, addr);
        printf("r[0]=%d r[1]=%d r[2]=%d r[3]=%d pc=%d sr=%d sp=%d clock=%d\n\n", r[0], r[1], r[2], r[3], pc, sr, sp, clock);
        //char c;
        //cin >> c;
    }
    
    if (debug) {
        for (j=0;j<limit;j++) {
            printf("%8d", mem[j]);
            if ((j%8)==7) printf("\n");
        }
        cout << endl;
    }
}

int VirtualMachine::load() {
    if (i) r[rd] = constant;
    else {
        r[rd] = mem[addr];
        clock += 3;
    }
}

int VirtualMachine::store() {
    mem[addr] = r[rd];
    clock += 3;
}


//http://teaching.idallen.com/dat2343/10f/notes/040_overflow.txt

/*
Be careful when handling sign extension. 
For example, if in loadi instruction CONST = 111111002 = -410, then to store it in some r[RD] register, 
it must be sign extended to 11111111111111002 (still -410). 
Sign extension occurs every time a short constant (in this case 8 bits) is assigned to a longer register (in this case 16 bits);
look for this every time negative numbers are involved. 
*/
int VirtualMachine::add() {
    if (i)
        r[rd] = r[rd] + constant;
    else
        r[rd] = r[rd] + r[rs];
    //Set carry 
    /*
    Register value & 1,0000,0000,0000,0000
    if r[rd] = 0?, sr 
    */
    if (r[rd]&0x10000) sr |= 01;
    else sr &= 0xe;
    // sign extend
    //1000,0000,0000,0000
    if (r[rd]&0x8000) r[rd] |= 0xffff0000;
    else r[rd] &= 0xffff;
}

int VirtualMachine::addc() {
    
}
int VirtualMachine::sub() {
    if (i) {
        r[rd] = r[rd] - constant;
        if (r[rd]&0x10000) sr |= 01;
    } else {
        r[rd] = r[rd] - r[rs];
        if (r[rd]&0x10000) sr |= 01;
    }
    
    return 0;
}

int VirtualMachine::subc() {
    
}

int VirtualMachine::And() {
    r[rd] = r[rd]&r[ri];
    return 0;
}

int VirtualMachine::Xor() {
    r[rd]= r[rd]^r[rs];
    return 0;
}

int VirtualMachine::Compl() {
    r[rd] = ~r[rd];
    return 0;
}

int VirtualMachine::shl() {
    r[rd] = r[rd] << 1;
    if (r[rd]&0x10000) sr |= 01;
    
    return 0;
}

int VirtualMachine::shla() {
    
}

int VirtualMachine::shr() {
    r[rd] = r[rd] >> 1;    
    
    if (r[rd]&0x10000) sr |= 01;    
}

int VirtualMachine::shra() {
    
}

int VirtualMachine::compr() {
    if (r[rd] < r[rs])
        //set less, reset equal/greater
    else if (r[rd] > r[rs])
    else if (r[rd] == r[rs])   
    
    else 
        error;
}

int VirtualMachine::getstat() {
    r[rd] = sr;
    return 0;
}

int VirtualMachine::putstat() {
    sr = r[rd];
    return 0;
}

int VirtualMachine::jump() {
    pc = addr;
    return 0;
}

int VirtualMachine::jumpl() {
    int temp = sr;
    if ( (temp&8) == 8)
        pc = addr;
    return 0;
}

int VirtualMachine::jumpe() {
    int temp = sr;
    if ( (temp&4) == 4) //if G =4
        pc = addr;
    return 0;
}

int VirtualMachine::jumpg() {
    int temp = sr;
    if ( (temp&2) == 2) //if G = 2
        pc = addr;
    //if G = 0, skip instruction                                                                                                                                                                                                                                   
    return 0;
}


/**
call and return instructions need special attention. 
As part of the execution of call instruction the status of VM must be pushed on to stack. 
Status of VM consists of pc, r[0]-r[3], and sr. 
The stack grows from the bottom of memory up, therefore initially sp = 256. 
After a call, sp is decremented by 6 as the values of pc, r[0]-r[3], and sr in the VM are pushed on to stack.
When a return instruction is executed, sp is incremented by 6 as values of pc, r[0]-r[3], and sr are popped from stack and restored in VM registers. 
When sp >= 256 stack is empty, and when sp < limit+6 stack is full. 

*/
int VirtualMachine::call() {
    //statusSaved is how much we save onto the stack for every call.
    //pc, sr, r[0-3] = 6 on memory.
    int statusSaved = 6;
    //Checks if sp is incorrect set or  
    // if sp would try to overwrite the instructions in memory.
    if (sp <= limit) {
        cout << "sp is incorrectly set!\n";
        return 1;
    }
    else if ( sp > (limit + statusSaved) ) {
        cout << "No more memory to save the current state for a call instruction";
        return 2;
    }
    mem[--sp]=pc;
    mem[--sp]=sr;
    mem[--sp]=r[3];
    mem[--sp]=r[2];
    mem[--sp]=r[1];
    mem[--sp]=r[0];
    clock +=3;    
    return 0;
}

int VirtualMachine::Return() {
    if (sp >= msize) {
        cout << "Stack is empty! \n";
        return 1;
    }
    r[0]=mem[sp++];
    r[1]=mem[sp++];
    r[2]=mem[sp++];
    r[3]=mem[sp++];
    sr=mem[sp++];
    pc=mem[sp++];
    clock +=3;    
}

int VirtualMachine::read() {
    getline(in, ???);
    clock +=27;    
}

int VirtualMachine::write() {
    out << instruction << endl;
    clock +=27;    
}

int VirtualMachine::halt() {
    exit;   //????Sounds good.
}

int VirtualMachine::noop() {
    return 0;    
}
