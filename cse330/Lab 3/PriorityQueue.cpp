//Audrey Cards 005980230
//CSE 330 Summer 2018
//Dr. Murphy 
//Lab 3 'PQ'


#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename t>
class pq
{
public:
pq();
t pqsize();
t getfront();
t printlist();
void addlist (t x);
void removefront();

private:
vector<t> v;
void sort (t x);

};

template <typename t>
pq<t>::pq()
{}
    
template <typename t>
t pq<t>::pqsize()
{
    return v.size();
}

template <typename t>
t pq<t>::getfront()
{
    return *v.begin();
}


template <typename t>
void sort(t x)
{
    for (int i = x.size()-1; i > 0; i--)
        for (int j = 0; j < 1; j++)
            if(x[j] > x[j+1])
                swap(x[j], x[j+1]);
}

template <typename t>
void pq<t>::addlist(t x)
{
    
    typename vector<t>::iterator ix = v.begin();
    while(ix != v.end() && x > *ix)
        {
            ix++;
        }
        v.insert (ix,x);
}

template <typename t>
t pq<t>::printlist()
{
    for (int i = 0; i < v.size(); i++)
            {
                cout << v[i] << " ";
            }
            cout << endl;
            
}

template <typename t>
void pq<t>:: removefront()
{
    assert(v.size() > 0);
    v.erase(v.begin());
}

int main()
{
    int p;
    pq<int> v;
    srand (time(0));
    for (int i = 0; i < 10; i++)
            v.addlist(rand());
        cout << v.getfront() << endl;
        cout << v.pqsize() << endl;
        v.printlist();
        v.removefront();
        v.printlist();
        cin >> p;
}

;

