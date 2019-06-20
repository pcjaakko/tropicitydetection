#ifndef TRAJECTORY_HH
#define TRAJECTORY_HH

#include <cassert>
#include <iostream>
#include <vector>

#include "auxiliary.hh"
#include "geometry3.hh"

using namespace std;

struct trajectory {
  vector<coord3d> positions;
  vector<coord3d> directions;
  double step_length;

  public:

  trajectory(const coord3d &pos, const coord3d &dir, const double eps = 0.01): step_length(eps) {positions.push_back(pos), directions.push_back(dir);}
  trajectory(const trajectory &traj): positions(traj.get_positions()), directions(traj.get_directions()), step_length(traj.get_step_length()) {}

  vector<coord3d> get_positions() const { return positions; }
  vector<coord3d> get_directions() const { return directions; }
  double get_step_length() const { return step_length; }
  double get_length() const { return positions.size(); }
  
  void append(const coord3d &pos, const coord3d &dir){
    positions.push_back(pos);
    directions.push_back(dir);
  }

  // extend trajectory by one element
  void extend();
  // extend trajectory until some criterion is met
  void complete();

  friend ostream& operator<<(ostream &s, const trajectory& T){
    s << fixed << T.positions << "," << T.directions << "," << T.step_length;
    return s;
  }

};

#endif

