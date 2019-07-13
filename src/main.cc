#include <vector>
#include <iostream>
#include <fstream>

#include "geometry3.hh"
#include "trajectory.hh"
#include "cube.hh"

#include <chrono>

using namespace std;
using namespace std::chrono;

int main () {

	//Cube cube("../jvec-minimal.vti");
	Cube cube("../QZVPPDh2te-m06-2x.vti");   
///*
	vector<vector<int>> tropplane = cube.gettropplaneZ(7.0);
	for (int i=0;i<tropplane.size();i++){
		cout<<tropplane[i]<<"\n";
	}	
	string filename="tallinn-tropicities.txt";
	cube.writetropplaneZ(filename,tropplane);
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
