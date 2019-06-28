#include <vector>
#include <iostream>
#include <fstream>
#include <list>

#include "geometry3.hh"
#include "trajectory.hh"

using namespace std;

int main () {
	coord3d position0(1,1,1);
	coord3d positionA(15,18.5,7);
	coord3d positionB(1.0001,1.0001,1.0001);
	coord3d positionC(1.00001,1.00001,1.00001);
	coord3d positionD(1.000001,1.000001,1.000001);
	coord3d positionX(2.741300e+02,-8.030197e+01,3.518843e+00);
	coord3d position3(10,10,10);

	//Cube cube("../jvec-minimal.vti");
	Cube cube("../QZVPPDh2te-m06-2x.vti");
	trajectory traj(positionA,cube.getvector(positionA),0.001);
	traj.complete(cube);
	traj.write2mathematicalist();
	/*cout << cube.getvector(coord3d(6.865040e+00,1.739498e+01,7.005310e+00));
	cout << cube.getvector(coord3d(6,17,7));
	cout << cube.getvector(coord3d(0,0,0));
	cout << cube.getvector(coord3d(1,0,0));*/
	//cout << cube.field;

	/*cout<<"\n\n\n";
	cout << "field size is: " << cube.field.size() << "\n";
	cout<<cube.field[0]<<"<---- this is first vector of field\n";
	cout<<cube.field[917]<<"<---- this is 917(+7 in .vti) vector of field\n";
	cout<<cube.field[917.802628]<<"<---- this is 917.802628(+7 in .vti) vector of field\n";
	cout<<cube.field[cube.field.size()-1]<<"<---- this is last vector of field\n";
	
*/
	//cout << traj;

	//cout << "getting vector at :             " << positionA << cube.getvector(positionA)<<"\n";
	//cout << "getting first vector from       " << positionX << cube.getvector(positionX)<<"\n";
	//cout << "getting first vector from       " << position3 << cube.getvector(position3)<<"\n";
	//cout << "trajectory classification is:   " << traj.classify(cube) <<"\n";

	return 0;
}
