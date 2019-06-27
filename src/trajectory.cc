
#include <cassert>
#include <iostream>
#include <vector>

#include "auxiliary.hh"
#include "geometry3.hh"
#include "trajectory.hh"

using namespace std;


void trajectory::extend(const Cube& cube){
  coord3d nextposition(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length);
  if (cube.outofbounds(nextposition)) {return;}
  append(nextposition,cube.getvector(nextposition));
}


void trajectory::printstatus(const Cube& cube){
    cout <<fixed <<"direction at position    "<<positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length << "was appended.\n";
}

void trajectory::complete(const Cube& cube){
  int i = 0;
  while (i<1000){
    extend(cube);
    if (i%100==0) {
      //printstatus(cube);
    }
    ++i;
  }
}

int trajectory::classify(const Cube& cube) const {
  coord3d bfield(0,0,1); //this should be read from a file
  double halfofsideofcube = pow(cube.field.size(), 1.0/3.0)/2;
  coord3d cubecenter(halfofsideofcube,halfofsideofcube,halfofsideofcube);
  coord3d crosssum(0,0,0);

  if (bfield.dot(crosssum) < 0) {
    return 0;
  }
  else if (bfield.dot(crosssum) > 0) {
    return 1;
  }
  else {
    return 2;
  }

}


bool trajectory::to_mathematica(const trajectory &t, FILE *file){
  ostringstream s;
  s << "traj = {{" << fixed << positions << "}};" << endl;
  s << "Graphics3D[{Arrow@#} & /@ data]" << endl;
  fputs(s.str().c_str(),file);

  return ferror(file) == 0;
}

