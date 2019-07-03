//Explanation for why 

#include <iostream>
#include <string>

int main () {
        int rd, constant, i=1;
        str >> rd >> constant;
        if (rd < 0 || rd > 3) return -1;
        if (constant < -128 || constant > 127) return -1;
        int inst = 0;
        
        inst = inst << 11| rd<<9 | i<<8 | (0x000000ff&constant);
        //Why do we need (0x000000ff&constant) instead of just constant?
        //Binary ANDing the constant and 1111,1111 gives only the constant if its a positive number
        //HOWEVER, for negative numbers, it does something different
        
        /*
        Example:
        loadi 2 -123
        C++ code: inst = 0 << 11| 2<<9 | 0<<8 | constant;
        
        Putting in -123 into the constant without the Binary-AND 0x000000ff 
        gives the instruction a total value of -123: 1111,1111,1000,0101
        
            StepbyStep
            0000,0000,0000,0000  | inst << 11
            0000,0100,0000,0000  | inst << 11 | 2 << 9 | 1 << 8
            1111,1111,1000,0101  | inst << 11 | 2 << 9 | 1 << 8 | constant
            The -123 overwrites the whole thing 
        
        Putting in -123 into the constant WITH the Binary-AND 0x000000ff 
        gives the instruction a total value of 1413:
        0000,0101,1000,0101
        
            0000,0000,0000,0000  | inst << 11
            0000,0101,0000,0000  | inst << 11 | 2 << 9 | 1 << 8        

            -123&0x000000ff 
            1111,1111,1000,0101  | - 123
        AND 0000,0000,1111,1111  | &0x000000ff 
            ---------------------| This removes the sign bits and places it into the constant.
            0000,0000,1000,0101  | == 133
        
            StepbyStep
            0000,0000,0000,0000  | inst << 11
            0000,0100,0000,0000  | inst << 11 | 2 << 9 | 1 << 8
            ----------------------
            0000,0101,1000,0101  | inst << 11 | 2 << 9 | 1 << 8 | constant
            == 1413
            
        */
        
        std::cout << inst;
    return 0;    
}