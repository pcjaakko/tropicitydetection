
#include <cassert>
#include <iostream>
#include <vector>

#include "auxiliary.hh"
#include "geometry3.hh"
#include "trajectory.hh"

#include <chrono>
using namespace std;
using namespace std::chrono;


void trajectory::extend_euler(const Cube& cube){  //Euler
  coord3d nextposition(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length);
  if (cube.outofbounds(nextposition)) {return;}
  append(nextposition,cube.getvector(nextposition));  
  }



/*
  void trajectory::extend_rungekutta(const Cube& cube){ //Runge-Kutta without chrono debugging
  coord3d k1 = cube.getvector(positions[positions.size()-1]).normalised()*step_length;
  coord3d v1 = cube.getvector(positions[positions.size()-1]+k1*0.5);
  coord3d k2 = v1.normalised()*step_length;
  coord3d v2 = cube.getvector(positions[positions.size()-1]+k2*0.5);
  coord3d k3 = v2.normalised()*step_length;
  coord3d v3 = cube.getvector(positions[positions.size()-1]+k3);
  coord3d k4 = v3.normalised()*step_length;
  coord3d nextposition(positions[positions.size()-1]+(k1+k2*2.0+k3*2.0+k4)/6.0);
  append(nextposition,cube.getvector(nextposition));  

}
*/

void trajectory::extend_rungekutta(const Cube& cube){ //Runge-Kutta with chrono debugging
	duration<double> getvector_span;
	duration<double> rungekutta_span;

  	steady_clock::time_point t1 = steady_clock::now();
  coord3d c1 = positions[positions.size()-1];
 	 steady_clock::time_point t2 = steady_clock::now();
  	rungekutta_span += duration_cast<duration<double>>(t2-t1); //<-- this is so crude :(

 	 t1 = steady_clock::now();
  coord3d k1 = cube.getvector(c1);
  	t2 = steady_clock::now();
 	 getvector_span += duration_cast<duration<double>>(t2-t1);
  

  	t1 = steady_clock::now();
  k1 = k1.normalised()*step_length;
  coord3d c2 = positions[positions.size()-1]+k1*0.5;
  	t2 = steady_clock::now();
  	rungekutta_span += duration_cast<duration<double>>(t2-t1);
  
  	t1 = steady_clock::now();
  coord3d v1 = cube.getvector(c2);
  	t2 = steady_clock::now();
  	getvector_span += duration_cast<duration<double>>(t2-t1);

 
  	t1 = steady_clock::now();
  coord3d k2 = v1.normalised()*step_length;
  coord3d c3 = positions[positions.size()-1]+k2*0.5;
  	t2 = steady_clock::now();
  	rungekutta_span += duration_cast<duration<double>>(t2-t1);

  	t1 = steady_clock::now();
  coord3d v2 = cube.getvector(c3);
  	t2 = steady_clock::now();
  	getvector_span += duration_cast<duration<double>>(t2-t1);

  
 	 t1 = steady_clock::now();
  coord3d k3 = v2.normalised()*step_length;
  coord3d c4 = positions[positions.size()-1]+k3;
 	 t2 = steady_clock::now();
 	 rungekutta_span += duration_cast<duration<double>>(t2-t1);
  
	  t1 = steady_clock::now();
  coord3d v3 = cube.getvector(c4);
  	t2 = steady_clock::now();
  	getvector_span += duration_cast<duration<double>>(t2-t1);
  
	  t1 = steady_clock::now();
  coord3d k4 = v3.normalised()*step_length;
  coord3d nextposition(positions[positions.size()-1]+(k1+k2*2.0+k3*2.0+k4)/6.0);
 	 t2 = steady_clock::now();
 	 rungekutta_span += duration_cast<duration<double>>(t2-t1);
  
	  t1 = steady_clock::now();
  coord3d c5 = cube.getvector(nextposition);
  	 t2 = steady_clock::now();
 	 getvector_span += duration_cast<duration<double>>(t2-t1);
  
  	t1 = steady_clock::now();
  append(nextposition,c5);  
 	t2 = steady_clock::now();
  	rungekutta_span += duration_cast<duration<double>>(t2-t1);

	//cout<<"\tRunge-Kutta took :o) "<<rungekutta_span.count()<<" seconds.\n";
	//cout<<"\tgetvector() took :o) "<<getvector_span.count()<<" seconds.\n\n";

}

void trajectory::printstatus(const Cube& cube){

//cout <<fixed <<"position: "<<positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length;
//cout <<"   vector before this: " << directions[positions.size()-1];
//cout<<"\n";
//cout << "veiictors: " << cube.getvector(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length)<<"\n"; 
} 
/*
void trajectory::complete(const Cube& cube){
  int i = 1;
  double dist2farthest=-1;
 
  //cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\n";
  //cout<<0.1*dist2farthest<<"\n";
 
  while ((positions[positions.size()-1]-positions[0]).norm()>0.2*dist2farthest && i<20000){ //if we get to a point that is less than a thousandth of the
  //while (i<20000){ //if we get to a point that is less than a thousandth of the
    //extend(cube);								//maximum distance of a point to the starting point, stop extending
    ////cout<<dist2farthest<<"\t\tdist2farthest\t";
    ////cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\t\tcurrentdisti";
    ////cout<<"\t"<<positions[positions.size()-1]<<"\n";    // ::-):-):-):-):-):-)-)
    ////cout<<"y's in da cube:"<<
    rungekutta(cube);
    if (cube.outofbounds(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length)){
      //cout<<"OUT OF BOUNDS!";
      oob = true;
      return;
    }
    if ((positions[positions.size()-1]-positions[0]).norm()>dist2farthest) {
      dist2farthest=(positions[positions.size()-1]-positions[0]).norm();
    }
    if (i%100==0){//i%500==0){ 
      ////cout<<"step no. " << i <<":\t";
      printstatus(cube);
    //cout<<dist2farthest<<"\t\tdist2farthest\t";
    //cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\t\tcurrentdist\n";
    }
    ++i;
  }
}
*/

void trajectory::complete(const Cube& cube){
  int step = 0;
  double dist2farthest = -1; //if this is set at 0 at declaration, the following while loop will never run

  //cout<<"\tBEGINNING OF TRAJ-DRAWING!  positions[0]: "<<positions[0]<<"\n";
		//duration<double> rk_span;
		//duration<double> getvector_span;
  while ((positions[positions.size()-1]-positions[0]).norm()>0.2*dist2farthest){ //if we get to a point that is less than SOME WELL-GUESSED FRACTION of the longest distance in the trajectory
    //printstatus(cube);
    ////cout<<dist2farthest<<"\t\tdist2farthest\t";
    ////cout<<(positions[positions.size()-1]-positions[0]).norm()<<"\t\tcurrentdist\n";
    ////cout<<"    step_length***"<<step_length<<"";
		//steady_clock::time_point rkstart = steady_clock::now();
    extend_rungekutta(cube);
		//steady_clock::time_point rkend = steady_clock::now();
		//rk_span += duration_cast<duration<double>>(rkend-rkstart);

    step++;
    
		//steady_clock::time_point getvector_start = steady_clock::now();
    if (cube.outofbounds(positions[positions.size()-1]+directions[directions.size()-1].normalised()*step_length)){
      //cout<<"OUT OF BOUNDS! t. trajectory.cc\n";
      oob = true;
      return;
    }
  
    if ((positions[positions.size()-1]-positions[0]).norm()>dist2farthest) {
      dist2farthest=(positions[positions.size()-1]-positions[0]).norm();
    }

    if (step>10000){ //a single trajectory must not be more than this many steps
      ////cout<<"we at "<<positions[0]<<",\t with step_length "<<step_length<<"\n";
      ////cout<<"resetting traj and increasing step_length...\n";
      step=0;
      step_length+=2;
      int size = positions.size();
      for (int a=0;a<size-1;a++){
        positions.pop_back();
        directions.pop_back();
      }
      dist2farthest = -1;
      ////cout<<"reset done. possize: "<<positions.size()<<" dirsize: "<<directions.size()<<"\n";
      ////cout<<"reset done. poos0:   "<<positions[positions.size()-1]<<" dir0:    "<<directions[directions.size()-1]<<"\n";
    }
		//steady_clock::time_point getvector_end = steady_clock::now();
		//getvector_span += duration_cast<duration<double>>(getvector_end-getvector_start);
  }
	/*//cout<<"\t ending traj-drawing...\n";
	//cout<<"\t\tRunge-Kutta took a total of "<<rk_span.count()<<" seconds.\n";
	//cout<<"\t\tGettin vecs took a total of "<<getvector_span.count()<<" seconds.\n";*/
   //print time spent on extending with runge-kutta
   //print total time spent on doing interpolations
}
      


int trajectory::classify(const Cube& cube, int bfielddir) const { 
  coord3d bfield;
  switch(bfielddir) {
    case 0:
      {
      bfield = coord3d(1,0,0);
      break;
      }
    case 1:
      {
      bfield = coord3d(-1,0,0);
      break;
      }
    case 2:
      {
      bfield = coord3d(0,1,0);
      break;
      }
    case 3:
      {
      bfield = coord3d(0,-1,0);
      break;
      }
    case 4:
      {
      bfield = coord3d(0,0,1);
      break;
      }
    case 5:
      {
      bfield = coord3d(0,0,-1);
      break;
      }
    default:
      {
      cout<<"bfielddir value wasn't 0-5.\n";
      return 7;
      }
  }
  
  coord3d crossum(0,0,0);

  if (oob==true) {return 0;}

  for (int i = 1; i<directions.size(); i++){
    crossum+=positions[i-1].cross(positions[i]);
  }
  crossum+=positions[positions.size()-1].cross(positions[0]);

  if (bfield.dot(crossum) < 0) { //counter-clockwise (paratropic) 
    return -1;
  }
  else if (bfield.dot(crossum) > 0) { //clockwise (diatropic)
    return 1;
  }
  else {                         //neither. something happened  // lnw: I suppose that never happens? // jaakko: i would also suppose that never happens, but in an _arbitary_ vector field such a trajectory could exist.
    return 2;
  }

}

void trajectory::write2mathematicalist(string filename) {
  ofstream outputfile;
  outputfile.open(filename);
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

