/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 1
 * Date: 10/14/2018
 * File: Assembler.cpp
**/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "Assembler.h"

using namespace std;

Assembler::Assembler() {
    lineCount = 0;
    
    jumpTable["load"]    = &Assembler::load;
    jumpTable["loadi"]   = &Assembler::loadi;
    jumpTable["store"]   = &Assembler::store;
    jumpTable["add"]     = &Assembler::add;
    jumpTable["addi"]    = &Assembler::addi;
    jumpTable["addc"]    = &Assembler::addc;
    jumpTable["addci"]   = &Assembler::addci;
    jumpTable["sub"]     = &Assembler::sub;
    jumpTable["subi"]    = &Assembler::subi;
    jumpTable["subc"]    = &Assembler::subc;
    jumpTable["subci"]   = &Assembler::subci;
    jumpTable["and"]     = &Assembler::And;
    jumpTable["andi"]    = &Assembler::andi;
    jumpTable["xor"]     = &Assembler::Xor;
    jumpTable["xori"]    = &Assembler::xori;
    jumpTable["compl"]   = &Assembler::Compl;
    jumpTable["shl"]     = &Assembler::shl;
    jumpTable["shla"]    = &Assembler::shla;
    jumpTable["shr"]     = &Assembler::shr;
    jumpTable["shra"]    = &Assembler::shra;
    jumpTable["compr"]   = &Assembler::compr;
    jumpTable["compri"]  = &Assembler::compri;
    jumpTable["getstat"] = &Assembler::getstat;
    jumpTable["putstat"] = &Assembler::putstat;
    jumpTable["jump"]    = &Assembler::jump;
    jumpTable["jumpl"]   = &Assembler::jumpl;
    jumpTable["jumpe"]   = &Assembler::jumpe;
    jumpTable["jumpg"]   = &Assembler::jumpg;
    jumpTable["call"]    = &Assembler::call;
    jumpTable["return"]  = &Assembler::Return;
    jumpTable["read"]    = &Assembler::read;
    jumpTable["write"]   = &Assembler::write;
    jumpTable["halt"]    = &Assembler::halt;
    jumpTable["noop"]    = &Assembler::noop;
}

int Assembler::assemble(fstream& in, fstream& out) {
    string line = "";
    string opcode = "";
    const int success = false;
    const int error = true;
    //const int debug = false;
    int instruction = 0;
    /*
        Changed while(!in.eof()) to while(getline(in,line)) 
        because while(getline...) checks each read to see if it works 
        rather than .eof simply checking for the end of file.
    */
    while(getline(in, line)) {
        istringstream str(line.c_str());
        str >> opcode;
        if(opcode[0] == '!') {
            continue;       //Forces execution of next loop
        }
        try {
            if(not jumpTable[opcode]) 
                throw NullPointerException();
            else {
                instruction = (this->*jumpTable[opcode])(str);
            }
        } catch (NullPointerException e) {
            cerr << e.what() << endl;
            return error;
        }
        if(instruction == -1) return error;
        out << instruction << endl;
        lineCount++;        //Added to count how many instructions there are.
    }
    return success;
}

int Assembler::load(istringstream & str) {
    int rd, addr;
    str >> rd >> addr;
    if(rd < 0 || rd > 3) return -1;
    if (addr < 0 || addr > 255) return -1;
    int inst = 0;
    return inst << 11 | rd << 9 | addr;
}

int Assembler::loadi(istringstream &str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd < 0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 0;
    return inst << 11 | rd << 9 | i << 8 | (0x000000ff&constant);
}

int Assembler::store(istringstream &str) {
    int rd, addr, i=1;
    str >> rd >> addr;
    if (rd < 0 || rd > 3) return -1;
    if (addr < 0 || addr > 255) return -1;
    int inst = 1;
    return inst << 11 | rd << 9 | i << 8 | addr;
}

int Assembler::add(istringstream & str){
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 2;
    return inst << 11 | rd << 9 | rs << 6 ;
}

int Assembler::addi(istringstream & str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd < 0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 2;
    return inst << 11| rd << 9 | i << 8 | (0x000000ff&constant);
}

int Assembler::addc(istringstream & str) {
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 3;
    return inst << 11 | rd << 9 | rs << 6;
}

int Assembler::addci(istringstream & str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd < 0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 3;
    return inst << 11| rd<<9 | i<<8 | (0x000000ff&constant);
}

int Assembler::sub(istringstream & str) {
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 4;
    return inst << 11 | rd << 9 |  rs << 6 ;
}

int Assembler::subi(istringstream & str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd <0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 4;
    return inst << 11| rd<<9 | i<<8 | (0x000000ff&constant);
}

int Assembler::subc(istringstream & str) {
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 5;
    return inst << 11 | rd << 9 | rs << 6;
}

int Assembler::subci(istringstream & str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd <0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 5;
    return inst << 11| rd<<9 | i<<8 | (0x000000ff&constant);
}

int Assembler::And(istringstream & str) {
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 6;
    return inst << 11 | rd << 9 | rs << 6;
}

int Assembler::andi(istringstream & str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd <0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 6;
    return inst << 11| rd<<9 | i<<8 | (0x000000ff&constant);
}

int Assembler::Xor(istringstream & str) {
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 7;
    return inst << 11 | rd << 9 | rs << 6;
}

int Assembler::xori(istringstream & str) {
    int rd, constant, i=1;
    str >> rd >> constant;
    if (rd <0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 7;
    return inst << 11| rd<<9 | i<<8 | (0x000000ff&constant);
}

int Assembler::Compl(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 8;
    return inst << 11 | rd << 9;
}

int Assembler::shl(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 9;
    return inst << 11 | rd << 9;
}

int Assembler::shla(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 10;
    return inst << 11 | rd << 9;
}

int Assembler::shr(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 11;
    return inst << 11 | rd << 9;
}

int Assembler::shra(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 12;
    return inst << 11 | rd << 9;
}

int Assembler::compr(istringstream & str) {
    int rd, rs;
    str >> rd >> rs;
    if (rd < 0 || rd > 3) return -1;
    if (rs < 0 || rs > 3) return -1;
    int inst = 13;
    return inst << 11 | rd << 9 | rs << 6;
}

int Assembler::compri(istringstream & str) {
    int rd, constant, i = 1;
    str >> rd >> constant;
    if (rd <0 || rd > 3) return -1;
    if (constant < -128 || constant > 127) return -1;
    int inst = 13;
    return inst << 11| rd << 9 | i << 8 | (0x000000ff&constant);
}

int Assembler::getstat(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 14;
    return inst << 11 | rd << 9;
}

int Assembler::putstat(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 15;
    return inst << 11 | rd << 9;
}

int Assembler::jump(istringstream & str) {
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255) return -1;
    int inst = 16;
    return inst << 11 | addr;
}

int Assembler::jumpl(istringstream & str) {
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255) return -1;
    int inst = 17;
    return inst << 11 | addr;
}

int Assembler::jumpe(istringstream & str) {
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255) return -1;
    int inst = 18;
    return inst << 11 | addr;
}

int Assembler::jumpg(istringstream & str) {
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255) return -1;
    int inst = 19;
    return inst << 11 | addr;
}

int Assembler::call(istringstream & str) {
    int addr;
    str >> addr;
    if (addr < 0 || addr > 255) return -1;
    int inst = 20;
    return inst << 11 | addr;
}

int Assembler::Return(istringstream & str) {
    int inst = 21;
    return inst << 11;
}

int Assembler::read(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 22;
    return inst << 11 | rd << 9;
}

int Assembler::write(istringstream & str) {
    int rd;
    str >> rd;
    if (rd < 0 || rd > 3) return -1;
    int inst = 23;
    return inst << 11 | rd << 9;
}

int Assembler::halt(istringstream & str) {
    int inst = 24;
    inst <<= 11;
    return inst;
}

int Assembler::noop(istringstream & str) {
    int inst = 25;
    inst <<= 11;
    return inst << 11;
}