
Conversation opened. 1 read message.

Skip to content
Using CoyoteMail Mail with screen readers
 
 
More 
2 of 255
 
Remaining Stuff
Inbox
	x
Yousef Jarrar
	
AttachmentsJul 29 (2 days ago)
	
to me
5 Attachments
	
Click here to Reply or Forward
Using 0.07 GB
Program Policies
Powered by
Google
Last account activity: 15 hours ago
Details
	
	

/*
Yousef Jarrar
Lab 7 CSE 330
Dr. Murphy
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

template <typename T>
class Tnode {
public:
	T data;
	Tnode *lptr;
	Tnode *rptr;
};

template <typename T>
class Mtree{
public:
	Mtree();
	void add(T x);
	bool find(T x);
	vector<T> inorderTrav();

private:
	Tnode<T> *root;
	int tsize;
	vector<T> v;
	void add(Tnode<T> *ptr, T x);
	bool find(Tnode<T> *ptr, T x);
	void inorderTrav(Tnode<T> *ptr);
};

template <typename T>
Mtree<T>::Mtree(){
	root = 0;
	tsize = 0;
}

template <typename T>
void Mtree<T>::add(Tnode<T> *ptr, T x){
	if (ptr->lptr == 0 && x <= ptr->data){
		Tnode<T> *nptr = new Tnode<T>;
		ptr->lptr = nptr;
		nptr->data = x;
		nptr->lptr = 0;
		nptr->rptr = 0;
		return;
	}
	if (ptr->rptr == 0 && x > ptr->data){
		Tnode<T> *nptr = new Tnode<T>;
		ptr->rptr = nptr;
		nptr->data = x;
		nptr->lptr = 0;
		nptr->rptr = 0;
		return;
	}
	if (x <= ptr->data){
		add(ptr->lptr, x);
	}
	else{
		add(ptr->rptr, x);
	}
}

template <typename T>
void Mtree<T>::add(T x){
	if (tsize == 0){
		Tnode<T> *ptr = new Tnode<T>;
		ptr->lptr = 0;
		ptr->rptr = 0;
		ptr->data = x;
		root = ptr;
	}
	else{
		add(root, x);
	}
	tsize++;
}

template <typename T>
bool Mtree<T>::find(Tnode<T> *ptr, T x){
	if (ptr == 0){
		return false;
	}
	else if (x == ptr->data){
		return true;
	}
	else if (x < ptr->data){
		find(ptr->lptr, x);
	}
	else if (x > ptr->data){
		find(ptr->rptr, x);
	}
}

template <typename T>
bool Mtree<T>::find(T x){
	if (tsize == 0){
		return false;
	}
	else{
		return find(root, x);
	}
}

template <typename T>
void Mtree<T>::inorderTrav(Tnode<T> *ptr){
	if (ptr == 0){
		return;
	}
	else{
		inorderTrav(ptr->lptr);
		v.push_back(ptr->data);
		inorderTrav(ptr->rptr);
	}
}

template <typename T>
vector<T> Mtree<T>::inorderTrav(){
	v.clear();
	if (tsize == 0){
		return v;
	}
	else{
		inorderTrav(root);
		return v;
	}
}

int main(){

	int find = 5;
	srand(time(NULL));

	clock_t start, finish;
	vector<int> v, v2, times;

	for (int i = 1000; i < 10001; i = i + 1000){
		for (int j = 0; j< 5; j++){
			start = clock();

			Mtree<int> m = Mtree<int>();

			for (int k = 0; k < i; k++){
				m.add(rand() % 100);
			}

			v = m.inorderTrav();

			for (int i = 0; i < v.size(); i++){
				cout << v[i] << ' ';
			}

			cout << endl;
			finish = clock();
			times.push_back(finish - start);
		}
		sort(times.begin(), times.end());
		v2.push_back(times[3]);
		times.clear();
		cout << "_______________________________________________________________________________________" << endl;
	}

	for (int l = 1000; l < 10001; l += 1000){
		cout << "The median time using " << l << " numbers in the tree was " << v2[(l / 1000) - 1] << "." << endl;
	}

	return 0;
}

mTree2.cpp
Displaying mTree1.cpp.