//Audrey Cards 005980230
//CSE 330 Summer 2018
//Dr. Murphy 
//Lab 5 

#include <iostream>
#include <cassert>
#include <time.h>

using namespace std;

template<typename T>
class mVec
{
public:
	void operator-- (T); //postfix
	void operator-- (); //prefix
	typedef T* iter;

	void insert(iter ix, T x);
	void erase(iter ix);

	mVec();
	mVec(unsigned int n);
	void pushBack(T x);
	void popBack();
	void clear();
	T operator[] (unsigned int i);
	int size();
	void print();
	iter begin(){return v;}
 	iter end(){return v+vecSize;}

private:
	int vecSize;
	int vecCap;
	T *v;
	void reserve(unsigned int n);
};

template<typename T>
mVec<T>::mVec()
{
	T *newV = new T[2];
	assert(newV != NULL);
	vecCap = 2;
	vecSize = 0;
	v = newV;
}

template<typename T>
mVec<T>::mVec(unsigned int n)
{
	T *newV = new T[n];
	//assert(newV != NULL);
	vecCap = n;
	vecSize = 0;
	//v = newV;
}

template<typename T>
void mVec<T>::pushBack(T x)
{
	if (vecSize < vecCap)
	{
		v[vecSize] = x;
		vecSize++;
	}
	else
	{
		this->reserve(2*vecCap);
		v[vecSize] = x;
		vecSize++;
	}
}

template<typename T>
void mVec<T>::popBack()
{
	assert(vecSize > 0);
	vecSize--;
}

template<typename T>
void mVec<T>::clear()
{
	vecSize = 0;
}

//remake with iterators
template<typename T>
void mVec<T>::insert(iter ix, T x)
{
	//assert(i >=0 && i<= vecSize);
	if (vecSize == vecCap)
	{
		this->reserve(2 * vecCap);
	}
	for (iter jx = v + vecSize; jx != ix; jx--)
	{
		*jx = *(jx-1);
	}
	*ix = x;
	vecSize++;
}

//remake with iterators
template<typename T>
void mVec<T>::erase(iter ix)
{
	//assert(ix  >= (--v) && ix < *(v + vecSize));
	for (iter jx = ix; jx != (v+vecSize)-1; jx++)
	{
		*jx = *(++jx);
	}
	vecSize--;
}

template<typename T>
T mVec<T>::operator[] (unsigned int i)
{
	assert(i < vecSize and vecSize > 0);
	return v[i];
}

template<typename T>
int mVec<T>::size()
{
	return vecSize;
}

template<typename T>
void mVec<T>::print()
{
	for (int i = 0; i < (this->vecSize); i++)
	{
		std::cout << this->v[i] << ' ';
		std::cout << std::endl;
	}
}

template<typename T>
void mVec<T>::reserve(unsigned int n)
{
	assert(n > vecCap);
	T *newV = new T[n];
	assert(newV != NULL);
	for (int i = 0; i < vecSize; i++)
	{
		newV[i] = v[i];
	}
	delete[] v;
	v = newV;
	vecCap = n;
}

template<typename T>
void mVec<T>::operator-- (T) //postfix
{
	this->popBack();
}

template<typename T>
void mVec<T>::operator-- () //prefix
{
	this->popBack();
}

int main()
{
	srand(time(NULL));
	mVec<int> x;
	int n = 15;

	for(int i=0;i< n;i++)
	{
		x.pushBack(rand()%100);
	}
	x.print();
	x.insert(x.begin() + 1, 5);
	x--;
	--x;

	cout << "Begin and End: " << *x.begin() << "/" << *(x.end()-1) << endl;
	cout << "------\n";
	x.print();

	return 0;
}
