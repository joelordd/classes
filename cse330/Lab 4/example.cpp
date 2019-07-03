#include <iostream>
#include "Vector.h"             //allows the use of Vector.h class

using namespace std;

int main {
    Vector::Vector<int> example;
    
    example.reserve(5);
    
    example.push_back(7);
    example.push_back(3);
    example.push_back(5);
    example.push_back(8);
    
    iterator iter = example.front();
    
    for(; *iter != example.back(); iter++) {
        cout << *iter << endl;
    }
    
    return 0;
}