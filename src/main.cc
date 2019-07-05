#include <vector>
#include <iostream>
#include <fstream>

#include "geometry3.hh"
#include "trajectory.hh"
#include "cube.hh"

using namespace std;

int main () {

	//Cube cube("../jvec-minimal.vti");
	Cube cube("../QZVPPDh2te-m06-2x.vti");   

	vector<vector<int>> tropplane = cube.gettropplaneZ(7.0, cube);
	for (int i=0;i<tropplane.size();i++){
		cout<<tropplane[i]<<"\n";
	}	
	string filename="tropicities.txt";
	cube.writetropplaneZ(filename,tropplane);


/*
	coord3d positionA(21,41,7);
	trajectory traj(positionA,cube.getvector(positionA),10);
	traj.complete(cube);
	traj.write2mathematicalist();
	cout <<"classification: "<<traj.classify(cube);
*/


cube.testfunc();
	return 0;
}
