#include <iostream>

using namespace std;

int main(){
    /*
    SR = 0000,0000,0000,1110
    //   ||||,||||,||DV,LEGC
    */
    //0x000E = 0000,1110 =  14
    //F0A8 == 61608
    int inst = 0xF0A8, sign=0, sign2;
    double x = 30.0
    float y = 30.000
    char 
    
    
    
    //sign=inst&0x8000;   //Keeps sign bit
    sign=inst;
    sign2 = sign;
    sign&=0x7FFF;
    sign2&=0x17FFF;
    
    inst >>= 1;

    //int sign2= inst|sign;
    
    //100010;
    cout << inst << endl;
    cout << sign << endl;
    cout << sign2 << endl;
    
    
    //1111,1000,0101,0100
    return 0;
}