//Audrey Cards 005980230
//CSE 330 Summer 2018
//Dr. Murphy 
//Lab 4 'mVec'

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

template <typename T>
class mVec {

public:
mVec();
mVec(unsigned int n);
void pushBack(T x);
void popBack();
void clear();
void insert(int i, T x);
void erase(int i);
T operator[] (unsigned int i);
int size();
void print();

private:
int vSize;
int vCap;
T *v;
void reserve(unsigned int n);
};

template <typename T>
mVec<T>::mVec(){
v = NULL;
vSize = 0;
reserve(10);
}

template <typename T>
mVec<T>::mVec(unsigned int n){
v = NULL;
vSize = 0;
reserve(n);
}

template <typename T>
//Hold twice the amount
void mVec<T>::pushBack(T x){
if(vSize == vCap)
reserve(vCap * 2);
v[vSize++] = x;
}

template <typename T>
void mVec<T>::popBack(){
if(vSize > 0){
vSize--;
}

}

template <typename T>
void mVec<T>::clear(){
vSize = 0;
}

template <typename T>
void mVec<T>::insert(int i, T x){
if(i >=0 && i < vSize){
if(vSize == vCap)
reserve(vCap * 2);

//move all elements 1 position to the right
for(int j = vSize-1; j >= i; j--)
v[j+1] = v[j];
v[i] = x;
vSize++;
}

}

template <typename T>
void mVec<T>::erase(int i){
if(i >=0 && i < vSize){
//move all elements 1 position left to cover the deleted element
for(int j = i+1; j < vSize; j++)
v[j-1] = v[j];
vSize--;
}

}

template <typename T>
T mVec<T>::operator[] (unsigned int i) {
return v[i];
}

template <typename T>
int mVec<T>::size() {
return vSize;
}

template <typename T>
void mVec<T>::reserve(unsigned int n){
T* temp = new T[n];
for(int i = 0; i < vSize; i++)
temp[i] = v[i];
if(v != NULL)
delete []v;
v = temp;
vCap = n;
}

template <typename T>
void mVec<T>::print(){
for(int i = 0; i < vSize; i++)
cout << v[i] << " ";
cout << endl << endl;
}

int main() {
mVec<int> numbers(5);
int x;
int Index;
cout << "size = " << numbers.size() << endl;
srand(time(NULL));
cout << "pushing 10 numbers" << endl;
for(int i = 1; i <= 10; i++){
x = rand() % 100;
cout << "pushing " << x << endl;
numbers.pushBack(x);
}

numbers.print();
x = rand() % 100;
Index = rand() % numbers.size();
cout << "inserting " << x << " at Index " << Index << endl;
numbers.insert(Index, x);
numbers.print();
Index = rand() % numbers.size();
cout << "erasing at Index " << Index << endl;
numbers.erase(Index);
numbers.print();
cout << "popBack 3 times" << endl;
for(int i = 1; i <= 3; i++)
numbers.popBack();
cout << "size = " << numbers.size() << endl;
numbers.print();
cout << "printing elements using [] operator" << endl;
for(int i =0; i < numbers.size(); i++)
cout << "numbers[" << i << "] = " << numbers[i] << endl;


}
