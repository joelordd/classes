/**
 * Names: Joseph Lord, Peter Chau
 * Project: Phase 3
 * Date: 12/2/2018
 * File: PT.h
 * 
 * PageTable keeps track of what frame each page is in.  There are three variables,
 *      frame: the frame that the page resides in
 *      valid: whether or not that frame is available
 *      modified: whether or not the page needs to be modified
**/

#ifndef PT_H
#define PT_H

#include <vector>

using namespace std;

// forward declarations
class OS;
class VirtualMachine;

class Row {
    private:
        int frame;
        bool valid;
        bool modified;
        
    friend
        class PT;
    friend
        class OS;
};

class PT {
    private:
        vector<Row> page_table;
        int ptSize;

    public:
        PT() { }
        PT(int );
        //void operator=(const PT &);
        void operator=(PT &);
        int log_to_phys(int );
    
    friend
        class OS;
    friend
        class VirtualMachine;
};

#endif