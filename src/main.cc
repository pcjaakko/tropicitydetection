#include <vector>
#include <iostream>
#include <fstream>
#include <list>

#include "geometry3.hh"
#include "cube.hh"

using namespace std;

int main () {
	Cube cube("jvec-minimal.vti");
	cout << "Let's do a dot product:   cube[0] . cube[1] = " << cube.field[0].dot(cube.field[1]) << "\n";
	cout << "Let's do a cross product: cube[0] x cube[1] = " << cube.field[0].cross(cube.field[1]) << "\n";
	cube.writecube("file.txt");
	cout << "getting first vector from cube:     " << cube.getvector({0,0,0})<<"\n";
	cout << "getting vector at (1,1,2) from cube:" << cube.getvector({1,1,2})<<"\n";
	return 0;
	//cout << "Dot product between the first and the second vector is: " << coord3d::cube.field[0].dot(cube.field[1]) << "\n";
}
