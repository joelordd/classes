/********************************************************************************
 * Joseph Lord
 * Priority_queue.h
 * 3/8/17
 * This program mimics the STL priority queue class.  Uses percolate and sift-down
 * techniques and vectors.
 ********************************************************************************/

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>

using namespace std;

//class that mimics the STL priority queue class.  Uses percolate and sift-down 
//techniques and vectors.
template <class T> 
class Priority_queue {
public:
   Priority_queue() : c() { }
   Priority_queue(const Priority_queue & p): c(p.c) { }
   Priority_queue & operator=(const Priority_queue & p) { c = p.c; return *this; }
   // vector destructor is automatically called when an obj of type Priority_queue is destroyed

   bool empty() { return c.empty(); }
   unsigned int size() { return c.size(); }
   
   //pushes a value onto the bottom of the heap and then sorts it by the percolate method
   void push(const T & x);
   
   //pops the highest number in the heap and then sorts heap again by making the
   //last number the root and using the sift down method on that number;
   void pop(); 
   T & top() { return c.front(); }
private:
   vector<T> c;
};

template <class T>
void Priority_queue<T>::push(const T & x)
{
   c.push_back(x);
   
   typename vector<T>::iterator start = c.begin();
   typename vector<T>::iterator stop = c.end();
   unsigned int i = (stop - start) - 1;         //position of x
   unsigned int parent = (i - 1) / 2;
   
   while(i > 0 && start[i] > start[parent]) {
      swap(start[i], start[parent]);
      
      i = parent;
      parent = (i - 1) / 2;
   }
}

template <class T>
void Priority_queue<T>::pop()
{
   typename vector<T>::iterator start = c.begin();
   typename vector<T>::iterator stop = c.end();
   unsigned int size = (stop - start) - 1;
   unsigned int i = 0;           // position
   
   swap(start[0], start[size]);
   
   while(i < size) {
      
      unsigned int childPosition = 2 * i + 1;
      
      if(childPosition < size) {
         
         if(childPosition + 1 < size && start[childPosition + 1] > start[childPosition])
            childPosition++;
         
         if(start[i] > start[childPosition]) {
            c.pop_back();
            return;
         }
         else
            swap(start[i], start[childPosition]);
      }
      
      i = childPosition;
   }
   
   c.pop_back();
}

#endif