#ifndef CUBE_HH
#define CUBE_HH


#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include "geometry3.hh"

using namespace std;

class Cube {
  public:
  vector<coord3d> field;
  int xrange;
  int yrange;
  int zrange;

  Cube(string filename);

  void writecube(const string& filename) const;

  double fieldlength() const;

  coord3d getvector(coord3d position) const;

  bool outofbounds (coord3d position) const;
    
};

#endif
