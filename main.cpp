#include<iostream>
using namespace std;
#include"stl_vector.h"
int main() {
	marxicb_stl::vector<int> Vector(2, 1);
	marxicb_stl::vector<int>::iterator t = Vector.begin();
	cout << *t;
	return 0;
}