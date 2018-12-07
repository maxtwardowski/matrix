#include "Matrix.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
	-> DONE exceptions
	-> DONE detach() investigation
	-> DONE basic arithmetic operators
	-> DONE copy constructor and assignment operator =
	-> DONE comparison operator ==
	-> DONE display operator <<
	-> DONE ability to read matrix from input file stream
	-> DONE single element access
*/

int main (int argc, char const *argv[]) {
	Matrix a, b(2, 2);

	ifstream filea("a.dat");
	filea >> a;

	b(0, 0) = 1;
	b(0, 1) = 1;
	b(1, 0) = 1;
	b(1, 1) = 1;

	cout << "a: " << endl << a << endl;
	cout << "b: " << endl << b << endl;

	Matrix c(2, 2), d(a), e(a);
	c = a - b;
	a -= b;

	cout << "AFTER:" << endl;
	cout << "a: " << endl << a << endl;
	cout << "b: " << endl << b << endl;
	cout << "c: " << endl << c << endl;
	cout << "d: " << endl << d << endl;
	cout << "e: " << endl << e << endl;

	Matrix x, y, z(2, 2), w(3, 8);
	ifstream filex("x.dat");
	ifstream filey("y.dat");

	filex >> x;
	filey >> y;

	z = x * y;
	x *= y;
	cout << "x: "<< endl << x << endl;
	cout << "y: "<< endl << y << endl;
	cout << "z: "<< endl << z << endl;

	cout << "x == y: " << (x == y) << endl;
	cout << "x != y: " << (x != y) << endl;

	filea.close();
	filex.close();
	filey.close();

	return 0;
}
