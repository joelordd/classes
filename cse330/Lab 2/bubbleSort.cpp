#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

//A function that makes a vector and sorts the numbers inside the vector using the bubble sort method.
int main()
{
    int n = 0;
    
    cout << "Enter the number of numbers to sort.\n";
    cin >> n;
    
    vector<int> v(n);
    
    srand(time(0));
    
    for(int i = 0; i < n; i++)
        v[i] = rand() % 1000000;
        
    for(int i = n-1; i > 0; i--) {
        for(int j = 0; j < i; j++) {
            if(v[i] > v[j])
                swap(v[i], v[j]);
        }
    }
    
    for(int k = 0; k < n; k++)
        cout << v[k] << endl;
        
    return 0;
}
