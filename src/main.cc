#include <vector>
#include <iostream>
#include <fstream>
#include <list>

#include "geometry3.hh"
#include "trajectory.hh"

using namespace std;

int main () {
	coord3d position0(1,1,1);
	coord3d positionA(30,20,7);
	coord3d positionB(1.0001,1.0001,1.0001);
	coord3d positionC(1.00001,1.00001,1.00001);
	coord3d positionD(1.000001,1.000001,1.000001);
	coord3d positionX(2.741300e+02,-8.030197e+01,3.518843e+00);
	coord3d position3(10,10,10);

	//Cube cube("../jvec-minimal.vti");
	Cube cube("../QZVPPDh2te-m06-2x.vti");
	trajectory traj(positionA,cube.getvector(positionA),0.2);
	traj.complete(cube);

	cout<<"\n\n\n";
	cout << "field size is: " << cube.field.size() << "\n";
	cout<<cube.field[0]<<"<---- this is first vector of field\n";
	cout<<cube.field[917]<<"<---- this is 917(+7 in .vti) vector of field\n";
	cout<<cube.field[917.802628]<<"<---- this is 917.802628(+7 in .vti) vector of field\n";
	cout<<cube.field[cube.field.size()-1]<<"<---- this is last vector of field\n";
	//traj.complete(cube);
	cout << traj;

	//cout << "getting vector at :             " << positionA << cube.getvector(positionA)<<"\n";
	//cout << "getting first vector from       " << positionX << cube.getvector(positionX)<<"\n";
	//cout << "getting first vector from       " << position3 << cube.getvector(position3)<<"\n";
	//cout << "trajectory classification is:   " << traj.classify(cube) <<"\n";

	return 0;
}
