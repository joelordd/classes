/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 2
 * Date: 10/22/2018
 * File: VirtualMachine.cpp
 * 
 * Added time slice to main function as well as exit status and i/o register
 * to the status register.  
 * The program counter is now equal to the program counter saved in the PCB 
 * added to the base of the process.  This maps the logical addresses to the
 * physical addresses at execution.
**/

#include "OS.h"
#include "VirtualMachine.h"

#include <iostream> 
#include <string>
#include <stdio.h>
#include <cstdlib>


using namespace std;

VirtualMachine::VirtualMachine () {
    opcode = 0, rd = 0, rs = 0, constant = 0, addr = 0;
    msize = 256;
    rsize = 4;
    clock = 0;
    stop = false;
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

int VirtualMachine::set_memory(const int &instruction, const int &index) {
    if (index < 0 || index > 256) return 1;
    mem[index] = instruction;
    return 0;
}

int VirtualMachine::setCarry(int reg) {
    if (reg&0x10000) sr |= 01;
    else sr &= 0x1E;
    return 0;
}

int VirtualMachine::getCarry() {
    int carry = sr&1;
    return carry;
}

int VirtualMachine::signExtend(int &reg) {
    if (reg&0x8000) reg |= 0xffff0000;
    else reg &= 0xffff;
    return 0;
}

int VirtualMachine::setOverflow(int x, int y, int reg) {
    //If overflow is detected, exit program
    if (x == y && x != (reg&0x8000)>>15) {
        sr |= 0x10;
        stop = true;
    } else sr &= 0xF;
    return 0;
}

int VirtualMachine::setStatus(int status) {
    //Saves the I/O and status bits, but clears the return bits.
    sr &= 0x31F;

    switch (status) {
        case 0: //time slice
            sr |= 0x00;
            break;
        case 1: //halt
            sr |= 0x20;
            break;
        case 2: //out of bound
            sr |= 0x40;
            break;
        case 3: //overflow
            sr |= 0x60; 
            break;
        case 4: //underflow
            sr |= 0x80;
            break;
        case 5: //invalid opcode
            sr |= 0xA0;
            break;
        case 6: //read
            sr |= 0xC0;
            break;
        case 7: //write
            sr |= 0xE0;
            break;
    }
    stop = true;
    return 0;
}

void VirtualMachine::run () {
    const int debug = false;
    int j;
    
    //cout << "==============================" << endl;
    //cout << "Virtual Machine is running";
    while (pc < limit && stop == false) {
        ir = mem[pc];
        //cout << "\nVirtual Machine running instruction: " << mem[pc] << endl;
        pc++;
        opcode = (ir&0xf800)>>11;
        rd = (ir&0x600)>>9;
        i = (ir&0x100)>>8;
        rs = (ir&0xc0)>>6;
        
        //cout << "PC is: " << pc << endl;

        //cout << "Reg1: " << r[1] << endl;
        //cout << "Reg2: " << r[2] << endl;
        
        addr=ir&0xff;
        constant = addr;
        if (ir&0x80) constant |= 0xffffff00; // if neg sign extend
        
        //cout << "Opcode is: " << opcode << endl;
        //cout << "Constant is: " << constant << endl;
        int current_inst = this->pc - 1;
        //cout << "Current Inst counter: " << current_inst << endl;

        clock++;
        //If opcode doesn't exist...
        if (opcode < 0 || opcode > 25) setStatus(5);
        
        (this->*jumpOpcode[opcode])();
        //if (opcode != 22) cout << "Finished accessing r[" << rd << "] = " << r[rd] << endl;

        //cout << "Reg0-2: " << r[0] << endl;
        //cout << "VM: Program has completed an instruction: \n";
        if (clock >= 15) setStatus(0);
    }
    //cout << "SR: " << sr << endl;
    //cout << "r[3]=" << r[3] << " , r[2] = " << r[2] << " , r[1] = " << r[1] << " , r[0] = " << r[0] << endl; 
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

//Added base value;
int VirtualMachine::load() {
    if (i) r[rd] = constant;        //loadi
    else {                          //load
        addr += base;
        
        //Address must be >= base and < base + limit.
        if (addr < base || addr >= base + limit) {    //out of bound
            setStatus(2);
            return 1;
        }
        
        r[rd] = mem[addr];
        clock += 3;
    }
    return 0;
}

int VirtualMachine::store() {
    addr += base;
    if (addr < base || addr >= base + limit) {    //out of bound
        setStatus(2);
        return 1;
    }
        
    mem[addr] = r[rd];
    clock += 3;
    
    return 0;
}

int VirtualMachine::add() {
    int sign1 = (r[rd]&0x80000)>>15;
    int sign2;
    if (i) {        //addi
        sign2 = (constant&0x8000)>>15;
        r[rd] = r[rd] + constant;
    } else {        //add
        sign2 = (r[rs]&0x8000)>>15;
        r[rd] = r[rd] + r[rs];
    }
    setCarry(r[rd]);
    setOverflow(sign1, sign2, r[rd]);
    signExtend(r[rd]);
    
    return 0;
}

int VirtualMachine::addc() {
    if (i) 
        r[rd] = r[rd] + constant + getCarry();    //addci       
    else 
        r[rd] = r[rd] + r[rs] + getCarry();     //addc
        
    setCarry(r[rd]);
    signExtend(r[rd]);  
    
    return 0;
}

int VirtualMachine::sub() {
    int sign1 = (r[rd]&0x8000)>>15;
    int sign2;
    if (i) {        //subi
        sign2 = (constant&0x8000)>>15;
        r[rd] = r[rd] - constant;
    } else {        //sub
        sign2 = (r[rs]&0x8000)>>15;
        r[rd] = r[rd] - r[rs];
    }
    setCarry(r[rd]);
    setOverflow(sign1, sign2, r[rd]);
    signExtend(r[rd]);

    return 0;
}

int VirtualMachine::subc() {
    if (i) 
        r[rd] = r[rd] - r[rs] - getCarry();     //subci
    else 
        r[rd] = r[rd] + - constant - getCarry();    //subc
    setCarry(r[rd]);
    signExtend(r[rd]);

    return 0;
}

int VirtualMachine::And() {
    if (i)                          //andi
        r[rd] = r[rd] & constant;
    else 
        r[rd] = r[rd] & r[rs];      //and    
    
    signExtend(r[rd]);
 
    return 0;
}

int VirtualMachine::Xor() {
    if (i) 
        r[rd] = r[rd]^constant;     //xori
    else 
        r[rd]= r[rd]^r[rs];         //xor
    
    signExtend(r[rd]);
    return 0;
}

int VirtualMachine::Compl() {
    r[rd] = ~r[rd];
    signExtend(r[rd]);
    return 0;
}

int VirtualMachine::shl() {
    r[rd] <<= 1;
    setCarry(r[rd]);
    signExtend(r[rd]);
    return 0;
}

int VirtualMachine::shla() {

    r[rd] <<= 1;                
    if (r[rd] & 0x10000) {  /* special set carry */
        sr |= 01;
        r[rd] |= 0x8000;
    } else {
        sr &= 0x1E;
        r[rd] &= 0x7FFF;
    }
    signExtend(r[rd]);

    return 0;
}

int VirtualMachine::shr() {
    r[rd] &= 0xFFFF;
    if (r[rd]&01) sr |= 01;
    else sr &= 0x1e;
    r[rd] >>= 1;
    
    return 0;
}

int VirtualMachine::shra() {
    //set carry
    if (r[rd]&01) sr |= 01;
    else sr &= 0x1e;
    r[rd] >>= 1;
    
    return 0;
}

int VirtualMachine::compr() {
    sr&=17;               //Temporarily saves overflow/carry flags
    if (i) {                        //compri
        if (r[rd] < constant) {     
            sr |= 8;                //Saves overflow/carry/less flag
            //cout << "Lesser i" << endl;            
        } else if (r[rd] > constant) {   
            sr |= 2;                //set greater, reset equal/less
            //cout << "Greater i" << endl;
        } else if (r[rd] == constant) { 
            sr |= 4;
            //cout << "Equal i" << endl;
        }
    } else {                        //compr
        if (r[rd] < r [rs]) {
            sr = 8;            
            //cout << "Lesser" << endl;
        } else if (r[rd] > r[rs]) {
            sr |= 2;
            //cout << "Greater" << endl;
        } else if (r[rd] == r[rs]) {                  
            sr |= 4;
            //cout << "Equal" << endl;
        }
    }    
    //cout << "SR: " << sr << endl;
    return 0;
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
    addr += base;
    if (addr < base || addr >= base + limit) {    //out of bound
        setStatus(2);
        return 1;
    }
    
    pc = addr;
    return 0;
}

int VirtualMachine::jumpl() {
    addr += base;
    if (addr < base || addr >= base + limit) {    //out of bound
        setStatus(2);
        return 1;
    }
    
    if (sr & 010) pc = addr;
    return 0;
}

int VirtualMachine::jumpe() {
    addr += base;
    if (addr < base || addr >= base + limit) {    //out of bound
        setStatus(2);
        return 1;
    }
    
    if (sr & 04) pc = addr;
    
    return 0;
}

int VirtualMachine::jumpg() {
    addr += base;
    if (addr < base || addr >= base + limit) {    //out of bound
        setStatus(2);
        return 1;
    }
    
    if (sr & 02) pc = addr;
    return 0;
}

int VirtualMachine::call() {
    if ( sp < (limit + 6) ) {
        setStatus(3);   //stack overflow
        return 1;
    }
    
    mem[--sp]=pc;
    mem[--sp]=r[3];
    mem[--sp]=r[2];
    mem[--sp]=r[1];
    mem[--sp]=r[0];
    mem[--sp]=sr;
    
    addr += base;
    if (addr < base || addr >= base + limit) {    //out of bound
        setStatus(2);
        return 1;
    }
    
    pc = addr;
    clock +=3;  
    
    return 0;
}

int VirtualMachine::Return() {
    if (sp > (256 - 6) ) {
        setStatus(4);   //stack underflow
        return 1;   
    }
    
    sr=mem[sp++];
    r[0]=mem[sp++];
    r[1]=mem[sp++];
    r[2]=mem[sp++];
    r[3]=mem[sp++];
    pc=mem[sp++];
    clock +=3;
    return 0;
}

int VirtualMachine::read() {
    //*input >> r[rd];  this will be done in the OS now

    //Sets I/O Register
    int temp = 0;
    temp = rd << 8;
    sr |= temp;
    setStatus(6);
    //cout << "SR: " << sr << endl;
    return 0;
}

int VirtualMachine::write() {
    //*output << r[rd] << endl; this will be done in the OS now

    //Sets I/O Register
    int temp = 0;
    temp = rd << 8;
    sr |= temp;    
    setStatus(7);
    //cout << "SR: " << sr << endl;

    return 0;
}

int VirtualMachine::halt() {
    setStatus(1);
    return 0;
}

int VirtualMachine::noop() {
    return 0;    
}
