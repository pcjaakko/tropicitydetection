
#include <cassert>
#include <iostream>
#include <vector>

#include "auxiliary.hh"
#include "geometry3.hh"
#include "trajectory.hh"

using namespace std;



void trajectory::extend(const Cube& cube){  //Euler
  coord3d nextposition(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length);
  if (cube.outofbounds(nextposition)) {return;}
  append(nextposition,cube.getvector(nextposition));  
  }



void trajectory::rungekutta(const Cube& cube){ //Runge-Kutta

}

void trajectory::printstatus(const Cube& cube){
    cout <<fixed <<"position: "<<positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length;
    cout <<"   vector before this: " << directions[positions.size()-1];
    cout<<"\n";//cout << "veiictors: " << cube.getvector(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length)<<"\n"; 
   
} 

void trajectory::complete(const Cube& cube){
  int i = 0;
  while (i<1000){
    extend(cube);
    if (i%100==0) {
      //cout<<"step no. " << i <<":\t";
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

void trajectory::write2mathematicalist() {
  ofstream outputfile;
  outputfile.open("trajectory.txt");
  outputfile<<"traj = {{";
  for (int i = 0; i<positions.size();i++) {    
    outputfile<<"{"<<positions[i][0]<<","<<positions[i][1]<<","<<positions[i][2]<<"}";
    if(i<positions.size()-1) {outputfile<<",";}
  }
  outputfile<<"}}";
}

bool trajectory::to_mathematica(const trajectory &t, FILE *file){
  ostringstream s;
  s << "traj = {{" << fixed << positions << "}};" << endl;
  s << "Graphics3D[{Arrow@#} & /@ data]" << endl;
  fputs(s.str().c_str(),file);

  return ferror(file) == 0;
}

