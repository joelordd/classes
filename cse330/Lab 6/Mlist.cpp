// Audrey Cards 005980230
//CSE 330 Summer 2018
// Dr. Murphy
// Lab 6 'Mlist'

#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
class Lnode{
  public :
  T data;
  Lnode *lPTR;
  Lnode *rPTR;
};

template <typename T>
class mList
{ public:
  mList();
  void add(T x);
  void del();
  T getfront();
  T getback();
  T operator[] (unsigned int i);
  void insert(int i,T x);
  void erase(int i);
  void del(T x);
  void print();

  private:
  Lnode<T> *first;
  Lnode<T> *last;
  int lsize;
};

template <typename T>
mList<T>::mList()
{
    first=0;
    last=0;
    lsize=0;
}

template <typename T>
void mList<T>::add(T x)
{
  Lnode<T> *ptr = new Lnode<T>;
  ptr->data=x;
  if(lsize==0)
    {
      ptr->lPTR=0;
      ptr->rPTR=0;
      first=ptr;
      last=ptr;
    }
  else
    {
      ptr->lPTR=last;
      ptr->rPTR=0;
      last->rPTR=ptr;
      last=ptr;
    }
  lsize++;
}


template <typename T>
void mList<T>::del()
{
  Lnode<T> *cur = new Lnode<T>;
  first=first->rPTR;
  delete cur;
  first->lPTR=0;
  lsize--;
}

template <typename T>
T mList<T>::getfront()
{
  return first->data;
}

template <typename T>
T mList<T>::getback()
{
  return last->data;
}

template <typename T>
T mList<T>::operator[] (unsigned int i)
{
  assert(lsize>0);
  Lnode<T> *ptr;
  ptr=first;
  for(int j=0; j<i; j++)
    {
      ptr=ptr->rPTR;
    }
  return ptr->data;
}

template <typename T>
void mList<T>::insert(int i,T x) {
Lnode<T> *nptr = new Lnode<T>;
nptr->data=x;
assert(i>=0 and i<=lsize);
  if (i==0 and lsize==0) //empty list
     {
       first=nptr;
       last=nptr;
       nptr->lPTR=0;
       nptr->rPTR=0;
       lsize++;
       return;
     }
   if (i==0) //add front
     {
       nptr->lPTR=0;
       nptr->rPTR=first;
       first=nptr;
       lsize++;
       return;
     }
   if (i==lsize) //add end
     {
       nptr->lPTR=last;
       last->rPTR=nptr;
       nptr->rPTR=0;
       last=nptr;
       lsize++;
       return;
     }

  Lnode<T> *ptr=first;
  for (int j=0; j< i; j++)
      {
           ptr=ptr->rPTR;
      }
Lnode<T> *previous = ptr->lPTR;
nptr->rPTR=ptr;
nptr->lPTR=previous;
previous->rPTR=nptr;
ptr->lPTR=nptr;
lsize++;
return;
}

template <typename T>
void mList<T>::erase(int i){
Lnode<T> *ptr=first;
  for (int j=0; j< i; j++)
      {
           ptr=ptr->rPTR;
      }
  // now ptr points to [i]
Lnode<T> *next = ptr->rPTR;
Lnode<T> *previous = ptr->lPTR;
   if (previous==0 and next==0) // one element
      { delete first;
            first=0;
             last=0;
              lsize--;
              return;
         }
  if ( next==0) // last element
      { last=previous;
        last->rPTR=0;
            delete ptr;
             lsize--;
              return;
        }
  if (previous==0) //delete first element
     { first=ptr->rPTR;
        first->lPTR=0;
         delete ptr;
          lsize--;
           ptr=first;
     }
      else // not the first and not the last
        {
             previous->rPTR=ptr->rPTR;
              next->lPTR=ptr->lPTR;
               delete ptr;
                lsize--;
               }
}

template <typename T>
void mList<T>::del(T x){
  if (lsize==0){
      return;
     }
Lnode<T> *ptr=first;
  while ( ptr != 0 ) {

           if (ptr->data == x){

         Lnode<T> *next = ptr->rPTR;
         Lnode<T> *previous = ptr->lPTR;
          if (previous==0 and next==0) {
           delete first;
                 first=0;
                   last=0;
                     lsize--;
                       return;
                       }
         if ( next==0) {
          last=previous;
              last->rPTR=0;
                  delete ptr;
                    lsize--;
                      return;
        }

         if (previous==0) {
              first=ptr->rPTR;
                    first->lPTR=0;
                      delete ptr;
                        lsize--;
        }
      else
        {
           previous->rPTR=ptr->rPTR;
           next->lPTR=ptr->lPTR;
           delete ptr;
           ptr=next;
           lsize--;
           }
       }
    else
  {
     ptr=ptr->rPTR;
    }
 }
}

template <typename T>
void mList<T>::print() {
    for(int i = 0; i < this->lsize; i++){
      cout << this->operator[](i)  << endl;
    }
}

int main() {
  mList<int> x;
  x.add(5);
  x.add(10);
  x.add(7);
  x.add(1);
  x.insert(3, 8);
  x.erase(1);
  x.del(7);
  x.print();
  return 0;
}
