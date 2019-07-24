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

//---------------below this line we're checking that the arguments are OK
bool errorsfound = false;

if (argc!=6) {
	cout<<"The program expects 5 arguments.\n";
	cout<<"1: Input file in .vti format\n";
	cout<<"2: Magnetic field direction:\n\t0 for x, 1 for -x,\n\t2 for y, 3 for -y,\n\t4 for z, 5 for -z,\n";
	cout<<"3: Fixed coord axle:\n\t0 for x,\n\t1 for y,\n\t2 for z.\n";
	cout<<"4: Fixed coord coordinate as a double. (For example 6.7 or 2.3e2)\n";
	cout<<"5: Output filename\n";
	cout<<"Example: ./main ../QZVPPDh2te-m06-2x.vti 4 2 6.7 output.txt\n";
	return 0;
}

try { double dubbel = stod(argv[4]); }
catch(invalid_argument) { cout<<"The fixed coordinate "<<argv[4]<<" was not a double in desired format.\n"; errorsfound = true; }

try { int integer1 = stoi(argv[2]); }
catch(invalid_argument) { cout<<"The magnetic field direction "<<argv[2]<<" was not an integer in desired format.\n"; errorsfound = true; }

try { int integer2 = stoi(argv[3]); }
catch(invalid_argument) { cout<<"The coord axle "<<argv[3]<<" was not an integer in desired format.\n"; errorsfound = true; }

if (stoi(argv[2])<0 || stoi(argv[2])>5){
	cout<<"Expected magnetic field direction:\n\t0 for x, 1 for -x,\n\t2 for y, 3 for -y,\n\t4 for z, 5 for -z,\n";
	cout<<"Entered value was: "<<argv[2]<<"\n";
	errorsfound = true;
}

if (stoi(argv[3])<0 || stoi(argv[3])>2) {
	cout<<"Expected coord axle:\n\t0 for x,\n\t1 for y,\n\t2 for z.\n";
	cout<<"Entered value was: "<<argv[3]<<"\n";
	errorsfound = true;  
}

if (errorsfound) {
	cout<<"Run the program without any parameters for operation manual.\n";
}
//---------------above this line we're checking that the arguments are OK


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
