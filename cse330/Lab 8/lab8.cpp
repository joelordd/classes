// Audrey Cards
// CSE 330 Summer 2018
// Dr. Murphy
// Lab 8




#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

int hash2(int n)
{
srand (n);
return rand();
}

int string2int(string s)
{
int h=0;
int d=1;
for (int i=0; i<s.length(); i++)
{
   char x=s[i];
   h+=(x)*d;
   d=d*10;
}
return h;
}

int main()
{

srand(time(NULL));
string s;
cout << "enter string:";
cin >> s;
cout << endl;
cout << string2int(s);
cout << endl;
cout << hash2(string2int(s));

cout << "\nInput r:";
int r;
cin >> r;
vector <int> v(r);

for (int i = 0; i<r; i++){
     int a = rand()%100;
     v[i] = a;
}
cout << "Input n:";
int n;
cin >> n;
vector <int> a(n);
for (int i = 0; i<n; i++){
     a[i] = -1;
     cout << a[i] << endl;
}

for (int i = 0; i<r; i++){
     cout << v[i] << endl;

     int ind = hash2(v[i])%n;
     cout << ind << " " << a[ind] << endl;
     if (a[ind] == -1){
        a[ind] = i;
     }
     else {
        cout << "There is a collision\n";
     }

}

}
