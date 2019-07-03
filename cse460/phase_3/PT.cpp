/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 3
 * Date: 12/2/2018
 * File: PT.cpp
 * 
 * PageTable keeps track of what frame each page is in.  There are three variables,
 *      frame: the frame that the page resides in
 *      valid: whether or not that frame is available
 *      modified: whether or not the page needs to be modified
 * This .cpp file is the function definitions as specified from the PT.h file.
**/

#include "PT.h"
#include <vector>
#include <iostream>

using namespace std;

PT::PT (int size){
    page_table = vector<Row>(size);
    ptSize = size;
    
    for (int i = 0; i < size; i++) {
        page_table[i].valid = false;
        page_table[i].modified = false;
    }
}

void PT::operator=(PT & p) {
    page_table = p.page_table;                  //for cloning pcb page table to tlb
}

// void PT::operator=(PT & p) {
//     swap(page_table, p.page_table);   
// }

int PT::log_to_phys(int addr) {                     //Addr mostly used as pc and is called from TLB. PC is now Logical
    //assuming 8 word pages
    int page_num = addr/8;                          //Checks what page an address is, there are 0-31 pages total, decimals are truncated
    
    if (page_table[page_num].valid) {
        int frame = page_table[page_num].frame;     //get frame from row
        int phys_addr = frame*8 + addr%8;           //Frame shifts left 3x, addr%8 = offset, gives rightmost 3 bits 

        return phys_addr;
    } else      // page fault
        return -1;                                  //lets vm know to trap to OS.
}