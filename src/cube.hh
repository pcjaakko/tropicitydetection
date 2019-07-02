#ifndef CUBE_HH
#define CUBE_HH

/*void printcoords(vector<coord3d> coords){
  cout << "Printing coords:\n";
  int i = 0;
  for (coord3d vec : coords) {
    ++i;
    cout << vec <<"\n";
  }
  cout << "There are " << i << " coord3d's inside the vector.\n";
}*/
  
class Cube {
  vector<coord3d> field;
  int xrange;
  int yrange;
  int zrange;

  public:
  void testfunc();
  Cube(string filename);
  void writecube(string filename);
  coord3d getvector(coord3d position) const;
  bool outofbounds (coord3d position) const; 
};


#endif
