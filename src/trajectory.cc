
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
  coord3d k1 = cube.getvector(positions[positions.size()-1])*step_length;
  coord3d v1 = cube.getvector(positions[positions.size()-1]+k1*0.5);
  coord3d k2 = v1*step_length;
  coord3d v2 = cube.getvector(positions[positions.size()-1]+k2*0.5);
  coord3d k3 = v2*step_length;
  coord3d v3 = cube.getvector(positions[positions.size()-1]+k3);
  coord3d k4 = v3*step_length;
  coord3d nextposition(positions[positions.size()-1]+(k1+k2*2.0+k3*2.0+k4)/6.0);
  append(nextposition,cube.getvector(nextposition));  

}

void trajectory::printstatus(const Cube& cube){
    cout <<fixed <<"position: "<<positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length;
    cout <<"   vector before this: " << directions[positions.size()-1];
    cout<<"\n";//cout << "veiictors: " << cube.getvector(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length)<<"\n"; 
   
} 

void trajectory::complete(const Cube& cube){
  int i = 1;
  double dist2farthest=-1;
 
  cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\n";
  cout<<0.1*dist2farthest<<"\n";
 
  while ((positions[positions.size()-1]-positions[0]).norm()>0.2*dist2farthest && i<100000 ){ //if we get to a point that is less than a thousandth of the
  //while (i<4000){ //if we get to a point that is less than a thousandth of the
    //extend(cube);								//maximum distance of a point to the starting point, stop extending
    //cout<<dist2farthest<<"\t\tdist2farthest\t";
    //cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\t\tcurrentdisti";
    //cout<<"\t"<<positions[positions.size()-1]<<"\n";    // ::-):-):-):-):-):-)-)
    //cout<<"y's in da cube:"<<
    rungekutta(cube);
    if (cube.outofbounds(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length)){
      cout<<"OUT OF BOUNDS!";
      oob = true;
      return;
    }
    if ((positions[positions.size()-1]-positions[0]).norm()>dist2farthest) {
      dist2farthest=(positions[positions.size()-1]-positions[0]).norm();
    }
    if (i%100==0){//i%500==0){ 
      //cout<<"step no. " << i <<":\t";
      printstatus(cube);
    cout<<dist2farthest<<"\t\tdist2farthest\t";
    cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\t\tcurrentdist\n";
    }
    ++i;
  }
}


int trajectory::classify(const Cube& cube) const { 
  coord3d bfield(0,0,1); //this should be read from a file
  coord3d crossum(0,0,0);

  if (oob==true) {return 0;}

  for (int i = 1; i<directions.size(); i++){
    crossum+=positions[i].cross(positions[i]-positions[i-1]);
  }

  if (bfield.dot(crossum) < 0) { //clockwise
    return -1;
  }
  else if (bfield.dot(crossum) > 0) { //counter-clockwise
    return 1;
  }
  else {                         //neither. something happened
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

