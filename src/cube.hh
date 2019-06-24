// writing on a text file
#include "geometry3.hh"
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <regex>


using namespace std;

vector <double> line2vec(string vecline) {
  vector<double> streamvec (3);
  return streamvec;
}


void printcoords(vector<coord3d> coords){
  cout << "Printing coords:\n";
  int i = 0;
  for (coord3d vec : coords) {
    ++i;
    cout << vec <<"\n";
  }
  cout << "There are " << i << " coord3d's inside the vector.\n";
}

  
class Cube {
  public:
  vector<coord3d> field;

  Cube(string filename){
    vector <string> coordinates;
    regex redata("[0-9]E");
    regex reblockstart("<DataArray");
    regex reblockend("</DataArray");
    smatch match;
    bool inblock = 0;

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
    }
  }

  void writecube(string filename) {
    cout << "The file-writing has not yet been implemented. " << filename << "\n";
  }


  coord3d getvector(coord3d position){
    coord3d intpos((int)position[0],(int)position[1],(int)position[2]);
    coord3d sumvec(0,0,0);
    double normsum = 0;
    double norm;
    for (int z=0; z<2; ++z) {
      for (int y=0; y<2; ++y) {
        for (int x = 0; x<2; ++x) {
          norm = (coord3d(intpos[0]+x,intpos[1]+y,intpos[2]+z)-position).norm();
          if (norm == 0.0) {
            return field[position[2]*(field.size()/3.0)+position[1]*(field.size()/9.0)+position[0]];
          }
          normsum += 1.0/norm;
          sumvec += getvector(coord3d(intpos[0]+x,intpos[1]+y,intpos[2]+z))/norm;
        }
      }
    }
    return sumvec/normsum;
  }


    
};

