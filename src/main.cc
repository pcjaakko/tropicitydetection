#include <vector>
#include <iostream>
#include <fstream>

#include "geometry3.hh"
#include "trajectory.hh"
#include "cube.hh"

#include <chrono>

using namespace std;
using namespace std::chrono;

int main (int argc, char* argv[]) {

if (argc!=6) {
	cout<<"The program expects 5 arguments.\n";
	cout<<"1: cubefile.vti\n";
	cout<<"2: Magnetic field direction:\n\t0 for x, 1 for -x,\n\t2 for y, 3 for -y,\n\t4 for z, 5 for -z,\n";
	cout<<"3: Fixed coord axle:\n\t0 for x,\n\t1 for y,\n\t2 for z.\n";
	cout<<"4: Fixed coord coordinate as a double. (For example 6.7)\n";
	cout<<"5: Output filename\n";
	cout<<"Example: ./main ../QZVPPDh2te-m06-2x.vti 4 2 6.7 output.txt\n";
	return 0;
}

	Cube cube(argv[1]);
	//Cube cube("../jvec-minimal.vti");
	//Cube cube("../QZVPPDh2te-m06-2x.vti");   
//	Cube cube("../jvec.vti");   


///*
	vector<vector<int>> tropplane = cube.gettropplane(argv[5], stoi(argv[2]), stoi(argv[3]), stod(argv[4]));
	for (int i=0;i<tropplane.size();i++){
		cout<<tropplane[i]<<"\n";
	}	
	cube.writetropplane(argv[5],tropplane);
//*/

/*	
	coord3d positionA(11,8,7);
	trajectory traj(positionA,cube.getvector(positionA),0.1);
	traj.complete(cube);
	traj.write2mathematicalist("tallinntraj.txt");
	cout <<"classification: "<<traj.classify(cube); 
*/
	return 0;
}
