#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <regex>
#include <cmath>
#include <unistd.h>
#include <sys/stat.h>

#include "geometry3.hh"
#include "cube.hh"
#include "trajectory.hh"

#include <chrono>

using namespace std;
using namespace std::chrono;

  
Cube::Cube(string filename){
  vector <string> coordinates;
  regex redata("[0-9]E");
  regex reblockstart("<DataArray");
  regex reblockend("</DataArray");
  smatch match;
  bool inblock = 0;
  regex imagedata("<ImageData");
  fstream vti (filename);
  string vtiline;
  if(!vti.good()){
    cout<<"Input file '"<<filename<<"' was not found.\n";
  }

  while (getline (vti, vtiline)) {
    if (inblock == 1 && regex_search(vtiline, match, redata)) {
      coordinates.push_back(vtiline);
      istringstream iss(vtiline);
      vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());
      coord3d doubleresults(stod(results[0]),stod(results[1]),stod(results[2]));
      field.push_back(doubleresults);
    }
    if (regex_search(vtiline, match, reblockstart)) {
      inblock = 1;
    }
    if (regex_search(vtiline, match, reblockend)) {
      inblock = 0;
    }
    if (regex_search(vtiline, match, imagedata)) {
      istringstream iss(vtiline);
      vector<string> results ((istream_iterator<string>(iss)),istream_iterator<string>());
      xrange = stoi(results[3])+1; 
      yrange = stoi(results[5])+1; 
      zrange = stoi(results[7])+1; 
    }
  }
}


void Cube::writecube(const string& filename) const {
  //cout << "File-writing has not yet been implemented. " << filename << "\n";
}


void Cube::testfunc(){
  //cout<<"TESTING HARD\n";
}
bool Cube::outofbounds (coord3d position) const {
  if (position[0]>xrange || position[1]>yrange || position[2]>zrange || position[0]<0 || position[1]<0 || position[2]<0) {
    return true;
  }
  return false;
}

coord3d Cube::getvector(coord3d position) const{ //linear interpolation
	//how much time does this take??
  if (outofbounds(position)) {
    return coord3d(7,7,7);
  }
  coord3d intpos((int)position[0],(int)position[1],(int)position[2]);
  coord3d sumvec(0,0,0);
  double normsum = 0;
  double norm;
  for (int z=0; z<2; ++z) {
    for (int y=0; y<2; ++y) {
      for (int x=0; x<2; ++x) {
        norm = (coord3d(intpos[0]+x,intpos[1]+y,intpos[2]+z)-position).norm();
        if (norm < 1e-12) { // lnw: it doesn't matter too much here, but in general it's dangerous to test for the equality with a double (because floating point inaccuracies).  The normal solution is eps=..., if( abs(norm) < eps ), where epsilon is chosen appropriately
        ////cout << "listindex: " <<fixed<<position[2]*xrange*yrange+position[1]*xrange+position[0] << "location at index: " <<  "\n"; 
        ////cout << field[position[2]*xrange*yrange+position[1]*xrange+position[0]] << "is the field at " << position << "\n";
          return field[position[2]*xrange*yrange+position[1]*xrange+position[0]];
        }
        normsum += 1.0/norm;
        sumvec += field[(intpos[2]+z)*xrange*yrange+(intpos[1]+y)*xrange+intpos[0]+x]/norm;
        ////cout << field[(intpos[2]+z)*xrange*yrange+(intpos[1]+y)*xrange+intpos[0]+x] << "is the field at {" <<intpos[0]+x<<","<<intpos[1]+y<<","<<intpos[2]+z<<"}" << "\n";
        }
      }
    }

  return sumvec/normsum;
}

coord3d Cube::getvector3(coord3d position) const{
  return coord3d(0,0,0);
}

vector<vector<int>> Cube::gettropplaneZ(double zcoord) const {
  vector<vector<int>>tropplaneZ;
  for (int y=0;y<yrange;y++) {
    vector<int> point_tropicity;
    tropplaneZ.push_back(point_tropicity);
    for (int x=0;x<xrange;x++){
      trajectory traj(coord3d(x,y,zcoord),getvector(coord3d(x,y,zcoord)),0.01);
      cout<<"\nNEW TRAJECTORY CREATED AT\t"<<x<<","<<y<<","<<zcoord<<"\n";
      traj.complete(*this);
      const string filename = "new-" + to_string(x) + "-" + to_string(y) + "-" + to_string_with_precision(zcoord) + ".txt";
      traj.write2mathematicalist(filename);
      tropplaneZ[y].push_back(traj.classify(*this, 4));
    }
  }
  return tropplaneZ;
}

void Cube::splitgrid(string gridfile, string weightfile, int bfielddir) const{

// lnw: could you define/explain what each of those are?
  vector<coord3d> gridpoints; //coordinates from the grid input file are read into this vector
  vector<double> gridweights; //weights from the weight input file are read into this vector
  vector<coord3d> isopoints;  //coordinates that were classified as isotropic are written into this vector
  vector<double> isoweights;  //and corresponding weights into this vector
  vector<coord3d> parapoints; //coordinates that were classified as paratropic are written into this vector
  vector<double> paraweights; //and corresponding weights into this vector
  vector<coord3d> zeropoints; //if a coordinate couldn't be classified (trajectory got out of bounds), it is written into this vector
  vector<double> zeroweights; //and the corresponding weight into this vector

  fstream grid (gridfile);
  string gridline;
  if(!grid.good()) { cout<<"Gridfile '"<<gridfile<<"' was not found.\n"; }
  while (getline (grid, gridline)) {
    istringstream gss(gridline);
    vector<string> gridresults((istream_iterator<string>(gss)),istream_iterator<string>());
    coord3d doublegridresults(stod(gridresults[0]),stod(gridresults[1]),stod(gridresults[2]));
    gridpoints.push_back(doublegridresults); 
  }

  fstream weights (weightfile);
  string weightsline;
  if(!weights.good()) { cout<<"Weightfile '"<<gridfile<<"' was not found.\n"; }
  while (getline (weights, weightsline)) {
    istringstream wss(weightsline);
    vector<string> weightsresults((istream_iterator<string>(wss)),istream_iterator<string>());
    gridweights.push_back(stod(weightsresults[0])); 
  }

//we now have the gridpoints in vector<coord3d> gridpoints and the corresponding weights in vector<double> gridweights
//next: get tropicity at each point, then write out two gridfiles

  for(int i=0;i<gridpoints.size();i++){
    trajectory traj(gridpoints[i],getvector(gridpoints[i]),0.01);
        cout<<"\nNEW TRAJECTORY CREATED AT\t"<<gridpoints[i]<<"\n";
    traj.complete(*this);
    int classification = traj.classify(*this, bfielddir);
    if (classification==-1){
      isopoints.push_back(gridpoints[i]);
      isoweights.push_back(gridweights[i]);
    } else if (classification==1){
      parapoints.push_back(gridpoints[i]);
      paraweights.push_back(gridweights[i]);
    } else if (classification==0){
      zeropoints.push_back(gridpoints[i]);
      zeroweights.push_back(gridweights[i]);
    } else {
      cout<<"couldn't classify this point :o(\n"; // lnw: you mean, could neither classify nor not classify this point? // jaakko: see trajectory.cc line 263: in case a trajectory is completed but its curvature is zero, trajectory::classify returns 2.
    }
  }
//now write iso, para and zero points and weights to respective files
  ofstream isopout, isowout, parapout, parawout, zeropout, zerowout;
  ostringstream isopoutfile;
  isopoutfile << gridfile << "-isotropic";
  isopout.open(isopoutfile.str());
  for (int i=0;i<isopoints.size();i++) {
    isopout<<isopoints[i][0]<<"\t"<<isopoints[i][1]<<"\t"<<isopoints[i][2]<<"\n";  
  }
  isopout.close();
  ostringstream isowoutfile;
  isowoutfile << weightfile << "-isotropic";
  isowout.open(isowoutfile.str());
  for (int i=0;i<isoweights.size();i++) {
    isowout<<isoweights[i]<<"\n";  
  }
  isowout.close();

  ostringstream parapoutfile;
  parapoutfile << gridfile << "-paratropic";
  parapout.open(parapoutfile.str());
  for (int i=0;i<parapoints.size();i++) {
    parapout<<parapoints[i][0]<<"\t"<<parapoints[i][1]<<"\t"<<parapoints[i][2]<<"\n";  
  }
  parapout.close();
  ostringstream parawoutfile;
  parawoutfile << weightfile << "-paratropic";
  parawout.open(parawoutfile.str());
  for (int i=0;i<paraweights.size();i++) {
    parawout<<paraweights[i]<<"\n";  
  }
  parawout.close();

  ostringstream zeropoutfile;
  zeropoutfile << gridfile << "-zerotropic";
  zeropout.open(zeropoutfile.str());
  for (int i=0;i<zeropoints.size();i++) {
    zeropout<<zeropoints[i][0]<<"\t"<<zeropoints[i][1]<<"\t"<<zeropoints[i][2]<<"\n";  
  }
  zeropout.close();
  ostringstream zerowoutfile;
  zerowoutfile << weightfile << "-zerotropic";
  zerowout.open(zerowoutfile.str());
  for (int i=0;i<zeroweights.size();i++) {
    zerowout<<zeroweights[i]<<"\n";  
  }
  zerowout.close();
}


vector<vector<int>> Cube::gettropplane(string filename, int bfielddir, int fixeddir, double fixedcoord) const {
  vector<vector<int>>tropplane;

  if (fixeddir==2) {
    for (int y=0;y<yrange;y++) {
    vector<int> point_tropicity;
    tropplane.push_back(point_tropicity);
      for (int x=0;x<xrange;x++){
        trajectory traj(coord3d(x,y,fixedcoord),getvector(coord3d(x,y,fixedcoord)),0.01);
        cout<<"\nNEW TRAJECTORY CREATED AT\t"<<x<<","<<y<<","<<fixedcoord<<"\n";
        traj.complete(*this);
      //const string filename = "new-" + to_string(x) + "-" + to_string(y) + "-" + to_string_with_precision(zcoord) + ".txt";
      //traj.write2mathematicalist(filename);
        tropplane[y].push_back(traj.classify(*this, bfielddir));
      }
    }
  return tropplane;
  }
 
  else if (fixeddir==1) {
    for (int z=0;z<zrange;z++) {
    vector<int> point_tropicity;
    tropplane.push_back(point_tropicity);
      for (int x=0;x<xrange;x++){
        trajectory traj(coord3d(x,fixedcoord,z),getvector(coord3d(x,fixedcoord,z)),0.01);
        cout<<"\nNEW TRAJECTORY CREATED AT\t"<<x<<","<<fixedcoord<<","<<z<<"\n";
        traj.complete(*this);
      //const string filename = "new-" + to_string(x) + "-" + to_string(y) + "-" + to_string_with_precision(zcoord) + ".txt";
      //traj.write2mathematicalist(filename);
        tropplane[z].push_back(traj.classify(*this, bfielddir));
      }
    }
  return tropplane;
  }

  else if (fixeddir==0) {
    for (int y=0;y<yrange;y++) {
    vector<int> point_tropicity;
    tropplane.push_back(point_tropicity);
      for (int z=0;z<zrange;z++){
        trajectory traj(coord3d(fixedcoord,y,z),getvector(coord3d(fixedcoord,y,z)),0.01);
        cout<<"\nNEW TRAJECTORY CREATED AT\t"<<fixedcoord<<","<<y<<","<<z<<"\n";
        traj.complete(*this);
      //const string filename = "new-" + to_string(x) + "-" + to_string(y) + "-" + to_string_with_precision(zcoord) + ".txt";
      //traj.write2mathematicalist(filename);
        tropplane[y].push_back(traj.classify(*this, bfielddir));
      }
    }
  return tropplane;
  }

  else {
    cout<<"FIXEDDIR was not 0-2.\n";
    vector<vector<int>> emptyvec;
    return emptyvec;
  }
}



void Cube::writetropplane(string filename, vector<vector<int>> tropicities) const{
  ofstream outputfile;
  outputfile.open(filename);
  outputfile<<"trop = {\n";
  for (int i=0;i<tropicities.size();i++){
    outputfile<<tropicities[i];
    if(i<tropicities.size()-1){outputfile<<",";}
    outputfile<<"\n";
  }
  outputfile<<"}";
}
 

