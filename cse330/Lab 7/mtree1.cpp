#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

using namespace std;

template <typename T>
class Tnode
{
public:
	T data;
	Tnode *lptr;
	Tnode *rptr;
};

template <typename T>
class Mtree
{
public:
	Mtree();
	void add(T x);
	bool find(T x);
	vector<T> inorderTrav();
	vector<T> preorderTrav();
	vector<T> postorderTrav();
private:
	Tnode<T> *root;
	int tsize;
	vector<T> v;
	void add(Tnode<T> *ptr, T x);
	bool find(Tnode<T> *ptr, T x);
	void inorderTrav(Tnode<T> *ptr);
	void preorderTrav(Tnode<T> *ptr);
	void postorderTrav(Tnode<T> *ptr);
};

template <typename T>
Mtree<T>::Mtree()
{
	root = 0;
	tsize = 0;
}

template <typename T>
void Mtree<T>::add(Tnode<T> *ptr, T x)
{
	if (ptr->lptr == 0 && x <= ptr->data)
	{
		Tnode<T> *nptr = new Tnode<T>;
		ptr->lptr = nptr;
		nptr->data = x;
		nptr->lptr = 0;
		nptr->rptr = 0;
		return;
	}
	if (ptr->rptr == 0 && x > ptr->data)
	{
		Tnode<T> *nptr = new Tnode<T>;
		ptr->rptr = nptr;
		nptr->data = x;
		nptr->lptr = 0;
		nptr->rptr = 0;
		return;
	}
	if (x <= ptr->data)
	{
		add(ptr->lptr, x);
	}
	else
	{
		add(ptr->rptr, x);
	}
}

template <typename T>
void Mtree<T>::add(T x)
{
	if (tsize == 0)
	{
		Tnode<T> *ptr = new Tnode<T>;
		ptr->lptr = 0;
		ptr->rptr = 0;
		ptr->data = x;
		root = ptr;
	}
	else
	{
		add(root, x);
	}
	tsize++;
}

template <typename T>
bool Mtree<T>::find(Tnode<T> *ptr, T x)
{
	if (ptr == 0)
	{
		return false;
	}
	else if (x == ptr->data)
	{
		return true;
	}
	else if (x < ptr->data)
	{
		find(ptr->lptr, x);
	}
	else if (x > ptr->data)
	{
		find(ptr->rptr, x);
	}
}

template <typename T>
bool Mtree<T>::find(T x)
{
	if (tsize == 0)
	{
		return false;
	}
	else
	{
		return find(root, x);
	}
}

template <typename T>
void Mtree<T>::inorderTrav(Tnode<T> *ptr)
{
	if (ptr == 0)
	{
		return;
	}
	else
	{
		inorderTrav(ptr->lptr);
		v.push_back(ptr->data);
		inorderTrav(ptr->rptr);
	}
}

template <typename T>
vector<T> Mtree<T>::inorderTrav()
{
	v.clear();
	if (tsize == 0)
	{
		return v;
	}
	else
	{
		inorderTrav(root);
		return v;
	}
}

template <typename T>
void Mtree<T>::preorderTrav(Tnode<T> *ptr)
{
	if (ptr == 0)
	{
		return;
	}
	else
	{
		v.push_back(ptr->data);
		preorderTrav(ptr->lptr);
		preorderTrav(ptr->rptr);
	}
}

template <typename T>
vector<T> Mtree<T>::preorderTrav()
{
	v.clear();
	if (tsize == 0)
	{
		cout << "Your tree is empty!!!" << endl;
		return v;
	}
	else
	{
		preorderTrav(root);
		return v;
	}
}

template <typename T>
void Mtree<T>::postorderTrav(Tnode<T> *ptr)
{
	if (ptr == 0)
	{
		return;
	}
	else
	{
		postorderTrav(ptr->lptr);
		postorderTrav(ptr->rptr);
		v.push_back(ptr->data);
	}
}

template <typename T>
vector<T> Mtree<T>::postorderTrav()
{
	v.clear();
	if (tsize == 0)
	{
		return v;
	}
	else
	{
		postorderTrav(root);
		return v;
	}
}

int main()
{
	Mtree<int> m = Mtree<int>();
	int find = 5;
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		m.add(rand() % 100 + 1);
	}
	vector<int> v;
	cout << "_________________________________________________________________" << endl;
	cout << "Random numbers using Inorder Traversal" << endl;
	v = m.inorderTrav();
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << ' ';
	}
	cout << endl;
	cout << "_________________________________________________________________" << endl;
	cout << "Random numbers using Preorder Traversal" << endl;
	v = m.preorderTrav();
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << ' ';
	}
	cout << endl;
	cout << "_________________________________________________________________" << endl;
	cout << "Random numbers using Postorder Traversal" << endl;
	v = m.postorderTrav();
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << ' ';
	}
	cout << endl;
	cout << "_________________________________________________________________" << endl;
	if(m.find(find))
	{
	std::cout << "The tree has a " << find << "."  << std::endl;
	}
	else
	{
	std::cout << "The tree does not have a " << find << "."  << std::endl;
	}
	return 0;
}

