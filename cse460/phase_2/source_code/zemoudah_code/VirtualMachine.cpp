#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include "VirtualMachine.h"

using namespace std;

int VirtualMachine::get_clock()
{
    return clock;
}

void VirtualMachine::run(fstream& objectCode, fstream& in, fstream& out)
{
    const int debug = false;
    int opcode, rd, i, rs, constant, addr, j;

    base = 0;
    for (limit = 0; objectCode >> mem[limit]; limit++);
    
    sr = 2;
    sp = msize;
    pc = 0;    
    cout << "==============================" << endl;
    cout << "Virtual Machine is running";
    while (pc < limit) {
        ir = mem[pc];        
        cout << "\nVirtual Machine running instruction: " << mem[pc] << endl;
        pc++;
        opcode = (ir&0xf800)>>11;
        rd = (ir&0x600)>>9;
        i = (ir&0x100)>>8;
        rs = (ir&0xc0)>>6;
        addr = ir&0xff;
        constant = addr;
        if (ir&0x80) constant |= 0xffffff00; // if neg sign extend 
        cout << "Opcode: " << opcode << endl;
        cout << "Constant: " << constant << endl;    
        
        int current_inst = this->pc - 1;
        cout << "Current Inst counter: " << current_inst << endl;
        
        clock++;
    
        if (opcode == 0) { /* load loadi */
            if (i) r[rd] = constant;
            else {
                if (addr >= limit) {
                    out << "Out of bound error.\n";
                    return;
                }
                r[rd] = mem[addr];
                clock += 3;
            }
        } else if (opcode == 1) { /* store */
            if (addr >= limit) {
                out << "Out of bound error.\n";
                return;
            }
            mem[addr] = r[rd];
            clock += 3;
        } else if (opcode == 2) { /* add addi */
            int sign1 = (r[rd]&0x8000)>>15;
            int sign2;
            if (i) {
                sign2 = (constant&0x8000)>>15;
                r[rd] = r[rd] + constant;
            } else {
                sign2 = (r[rs]&0x8000)>>15;
                r[rd] = r[rd] + r[rs];
            }
            // set CARRY
            if (r[rd]&0x10000) sr |= 01;
            else sr &= 0x1e;
            // set OVERFLOW
            if (sign1 == sign2 and sign1 != (r[rd]&0x8000)>>15)
                sr |= 0x10;
            else
                sr &= 0xf;
            // sign extend
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 3) { /* addc addci */
            if (i)
                r[rd] = r[rd] + constant + sr&01;
            else
                r[rd] = r[rd] + r[rs] + sr&01;
            if (r[rd]&0x10000) sr |= 01;
            else sr &= 0x1e;
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 4) { /* sub subi */
            int sign1 = (r[rd]&0x8000)>>15;
            int sign2;
            if (i) {
                sign2 = (constant&0x8000)>>15;
                r[rd] = r[rd] - constant;
            } else {
                sign2 = (r[rs]&0x8000)>>15;
                r[rd] = r[rd] - r[rs];
            }
            // set CARRY
            if (r[rd]&0x10000) sr |= 01;
            else sr &= 0x1e;
            // set OVERFLOW
            if (sign1 != sign2 and sign2 == (r[rd]&0x8000)>>15)
                sr |= 0x10;
            else
                sr &= 0xf;
            // sign extend
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 5) { /* subc subci */
            if (i)
                r[rd] = r[rd] - constant - sr&01;
            else
                r[rd] = r[rd] - r[rs] - sr&01;
            if (r[rd]&0x10000) sr |= 01;
            else sr &= 0x1e;
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 6) { /* and andi */
            if (i) r[rd] = r[rd] & constant;
            else r[rd] = r[rd] & r[rs];
            // sign extend
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 7) { /* xor xori */
            if (i) r[rd] = r[rd] ^ constant;
            else r[rd] = r[rd] ^ r[rs];
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 8) { /* compl */
            r[rd] = ~r[rd];
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 9) { /* shl */
            r[rd] <<= 1;
            if (r[rd]&0x10000) sr |= 01;
            else sr &= 0x1e;
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 10) { /* shla */
            r[rd] <<= 1;
            if (r[rd]&0x10000) {
                sr |= 01;
                r[rd] |= 0x8000;
            } else {
                sr &= 0x1e;
                r[rd] &= 0x7fff;
            }
            if (r[rd]&0x8000) r[rd] |= 0xffff0000;
            else r[rd] &= 0xffff;
        } else if (opcode == 11) { /* shr */
            r[rd] &= 0xffff;
            if (r[rd]&01) sr |= 01;
            else sr &= 0x1e;
            r[rd] >>= 1;
        } else if (opcode == 12) { /* shra */
            if (r[rd]&01) sr |= 01;
            else sr &= 0x1e;
            r[rd] >>= 1;
        } else if (opcode == 13) { /* compr  compri */
            sr &= 021;
            if (i) {
                if (r[rd]<constant) sr |= 010;
                if (r[rd]==constant) sr |= 04;
                if (r[rd]>constant) sr |= 02;
            } else {
                if (r[rd]<r[rs]) sr |= 010;
                if (r[rd]==r[rs]) sr |= 04;
                if (r[rd]>r[rs]) sr |= 02;
            }
            cout << "SR: " << sr << endl;

        } else if (opcode == 14) { /* getstat */
            r[rd] = sr;
        } else if (opcode == 15) { /* putstat */
            sr = r[rd];
        } else if (opcode == 16) { /* jump */
            if (addr >= limit) {
                out << "Out of bound error.\n";
                return;
            }
            pc = addr;
        } else if (opcode == 17) { /* jumpl */
            if (addr >= limit) {
                out << "Out of bound error.\n";
                return;
            }
            if (sr & 010) pc = addr;
        } else if (opcode == 18) { /* jumpe */
            if (addr >= limit) {
                out << "Out of bound error.\n";
                return;
            }
            if (sr & 04) pc = addr;
        } else if (opcode == 19) { /* jumpg */
            if (addr >= limit) {
                out << "Out of bound error.\n";
                return;
            }
            if (sr & 02) pc = addr;
        } else if (opcode == 20) { /* call */
            if (addr >= limit) {
                out << "Out of bound error.\n";
                return;
            }
            if (sp < limit+6) {
                cout << "Stack Overflow\n";
                exit(1); // stack overflow
            }
            mem[--sp] = pc;
            for (j=0; j<4; j++)
                mem[--sp] = r[j];
            mem[--sp] = sr;
            pc = addr;
            clock += 3;
        } else if (opcode == 21) { /* return */
            if (sp > 256-6) {
                cout << "Stack Underflow\n";
                exit(2); // stack underflow
            }
            sr = mem[sp++];
            for (j=3; j>=0; j--)
                r[j] = mem[sp++];
            pc = mem[sp++];
            clock += 3;
            cout << "SR: " << sr << endl;

        } else if (opcode == 22) { /* read */
            in >> r[rd];
            // assert r[rd] is in the allowed range
            cout << "SR: " << sr << endl;
            clock += 27;
            cout << "OS: Finished accessing r[" << rd << "] = " << r[rd] << endl;

        } else if (opcode == 23) { /* write */
            out << r[rd];
            clock += 27;
            cout << "\nWriting to r[" << rd << "] value: " << r[rd] << endl;
            cout << "SR: " << sr << endl;
        } else if (opcode == 24) { /* halt */
            break;
        } else if (opcode == 25) { /* noop */
            /* do nothing */
        } else {
            cout << "Bad opcode = " << opcode << endl;
            exit(3);
        }
        cout << "VM: Program has completed an instruction: \n";
        cout << "Finished accessing r[" << rd << "] = " << r[rd] << endl;
        if (debug) {
            printf("ir=%d op=%d rd=%d i=%d rs=%d const=%d addr=%d\n", ir, opcode, rd, i, rs, constant, addr);
            printf("r[0]=%d r[1]=%d r[2]=%d r[3]=%d pc=%d sr=%d sp=%d clock=%d\n\n", r[0], r[1], r[2], r[3], pc, sr, sp, clock);
            //char c;
            //cin>>c;
        }
    }
    cout << "SR: " << sr << endl;
    cout << "r[3]=" << r[3] << " , r[2] = " << r[2] << " , r[1] = " << r[1] << " , r[0] = " << r[0] << endl; 
    if (debug) {
        for (j=0; j<limit; j++) {
            printf("%8d", mem[j]);
            if ((j%8)==7) printf("\n");
        }
        cout << endl;
    }
} /* main */