//Shifting numbers test 
//Demonstrates how shifting numbers work

#include <iostream>
#include <string>

using namespace std;

int shiftLeft (int num, int shift){
    num = num << shift;
    return num;
}

int decimalToBinary (int decimal)
{
    long long binaryNumber = 0;
    int remain, i = 1;

    while (decimal !=0)
    {
        //cout << decimal << "/2, Remainder = " << decimal %2 << ", Quotient = " << decimal/2 << endl;
        remain = decimal %2;
        decimal /= 2;
        binaryNumber += remain*i;
        i *= 10;
    }
    return binaryNumber;
    
}

int main () {
    
    //0x000000ff&0
    int opcode0=21, rd0 = 0, addr0=0;//addr0=1&0x000000ff&0;
    int const i0=0, i1=1;
    int opcode1=4, rd1=2, addr1=1;
    
    int addrN=(addr1&0x000000ff);
    /*
    Inst1 format: 
        00000|00|0|00|000000
    Inst2 format:
        00000|00|0|00000000
    */
    int inst0 = opcode0 << 11 | rd0 << 9 | i0 << 8 | addr0; //| addr0;
    int inst1 = opcode1  << 11 |rd1 << 9 | i1 << 8 | addrN;
    cout << inst0 << endl;
    //cout << inst1 << endl;
    cout << inst1 << endl;

    //cout << decimalToBinary(inst1) << endl;
    return 0;

}
