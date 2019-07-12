#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <regex>
#include <cmath>

#include "geometry3.hh"
#include "cube.hh"
#include "trajectory.hh"

using namespace std;

  
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

  while (getline (vti, vtiline)) {
    if (inblock == 1) {
      if (regex_search(vtiline, match, redata)) {
        //cout << vtiline<<":o)\n";
        coordinates.push_back(vtiline);
        istringstream iss(vtiline);
        vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());
        coord3d doubleresults(stod(results[0]),stod(results[1]),stod(results[2]));
        field.push_back(doubleresults);
      }
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
  cout << "File-writing has not yet been implemented. " << filename << "\n";
}


void Cube::testfunc(){
  cout<<"TESTING HARD\n";
}
bool Cube::outofbounds (coord3d position) const {
  if (position[0]>xrange || position[1]>yrange || position[2]>zrange || position[0]<0 || position[1]<0 || position[2]<0) {
    return true;
  }
  return false;
}

coord3d Cube::getvector(coord3d position) const{ //linear interpolation
  if (outofbounds(position)) {
    return coord3d(7777777,7777777,7777777);
  }
  coord3d intpos((int)position[0],(int)position[1],(int)position[2]);
  coord3d sumvec(0,0,0);
  double normsum = 0;
  double norm;
  for (int z=0; z<2; ++z) {
    for (int y=0; y<2; ++y) {
      for (int x=0; x<2; ++x) {
        norm = (coord3d(intpos[0]+x,intpos[1]+y,intpos[2]+z)-position).norm();
        if (norm == 0.0) {
        //cout << "listindex: " <<fixed<<position[2]*xrange*yrange+position[1]*xrange+position[0] << "location at index: " <<  "\n"; 
        //cout << field[position[2]*xrange*yrange+position[1]*xrange+position[0]] << "is the field at " << position << "\n";
          return field[position[2]*xrange*yrange+position[1]*xrange+position[0]];
        }
        normsum += 1.0/norm;
        sumvec += field[(intpos[2]+z)*xrange*yrange+(intpos[1]+y)*xrange+intpos[0]+x]/norm;
        //cout << field[(intpos[2]+z)*xrange*yrange+(intpos[1]+y)*xrange+intpos[0]+x] << "is the field at {" <<intpos[0]+x<<","<<intpos[1]+y<<","<<intpos[2]+z<<"}" << "\n";
        }
      }
    }

  return sumvec/normsum;
}



vector<vector<int>> Cube::gettropplaneZ(double zcoord) const {
  vector<vector<int>>tropplaneZ;
  cout<<"EMPTY PLANE CREATED\n";
  for (int y=0;y<yrange;y++) {
    vector<int> vektori;
    tropplaneZ.push_back(vektori);
    for (int x=0;x<xrange;x++){
      trajectory traj(coord3d(x,y,zcoord),getvector(coord3d(x,y,zcoord)),0.01);
      cout<<"\nNEW TRAJECTORY CREATED AT\t"<<x<<","<<y<<","<<zcoord<<"\n";
      traj.complete(*this);
      const string filename = "new-" + to_string(x) + "-" + to_string(y) + "-" + to_string_with_precision(zcoord) + ".txt";
      traj.write2mathematicalist(filename);
      tropplaneZ[y].push_back(traj.classify(*this));
      //tropplaneZ[y].push_back(1);
    }
  }
  return tropplaneZ;
}

void Cube::writetropplaneZ(string filename, vector<vector<int>> tropicities) const{
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
