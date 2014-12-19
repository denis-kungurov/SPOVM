#include <iostream>
#include <string>
#include "ptr.h"

class MyClass
{
public:
	MyClass() {};
	~MyClass() {};

	double return_square(int number) { return number*number; }
};

void test1(Ptr<MyClass> ptr)
{
	std::cout << ptr->return_square(5) << std::endl;
}

void test2(Ptr<MyClass> ptr)
{
	Ptr<MyClass> b;
	b = ptr;
	std::cout << b->return_square(9) << std::endl;
}


int main(int argc, char const *argv[])
{
	Ptr<MyClass> a;
	std::cout << a->return_square(2) << std::endl;
	Ptr<MyClass> b = a;
	test1(a);
	test2(a);

	getchar();
	return 0;
}