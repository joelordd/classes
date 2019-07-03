// Audrey Cards 005980230
// CSE 330 Summer 2018
// Dr. Murphy 


#include <iostream>
#include <cstdlib>
#include <vector>
#include "time.h"

using namespace std;

void selectionSort(vector<int>& v) {
    for(int i = 0; i < v.size(); i++) {
        for(int j = i+1; j < v.size(); j++) {
            if(v[i] > v[j])
                swap(v[i], v[j]);
        }
    }
    
    /*
    for(int k = 0; k < v.size(); k++)
        cout << v[k] << endl;
    */
}

void bubbleSort(vector<int>& v) {
    for(int i = v.size(); i > 0; i--) {
        for(int j = 0; j < i; j++) {
            if(v[i] > v[j])
                swap(v[i], v[j]);
        }
    }
    
    /*
    for(int k = 0; k < v.size(); k++)
        cout << v[k] << endl;
    */
}

int main() {
    int stop = 5;
   
    int SelAvg = 0;
    int BubAvg = 0;
	
	int n=0;
	
	cout << "Enter size of input :" ;
	cin >> n;
	
	while(stop != 0) {
	    
	vector<int> v;
	
	clock_t start, finish;
    srand (time(NULL));
    
	for (int i=0; i<n; i++)
		v.push_back(rand());
	
	start = clock();
	selectionSort(v);
	finish = clock();
	cout << "Sel Sort CPU Time = " << finish-start << endl;
	SelAvg += finish-start;
	
	start = clock();
	bubbleSort(v);
	finish = clock();
	cout << "Bubble Sort CPU Time = " << finish-start << endl <<endl;
	BubAvg += finish-start;
	stop--;
    }
    
    SelAvg /= 5;
    BubAvg /= 5;
    
    cout << "S-Avg: " << SelAvg << "\n B-Avg: " << BubAvg << endl;
	return 0;
}

//S-Avg
//1000- 9146
//2000-37809
//3000-84233
//4000-147658
//5000-233329
//6000-338541
//7000-465489
//8000-602408
//9000-762231
//10000-945860

//B-Avg
//1000-3168
//2000-13113
//3000-28147
//4000-49812
//5000-79476
//6000-113608
//7000-155597
//8000-203350
//9000-260471
//10000-317494
