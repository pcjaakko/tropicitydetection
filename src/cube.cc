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
      origin.push_back(stod(results[10]));
      origin.push_back(stod(results[11]));
      origin.push_back(stod(results[12]));
      spacing.push_back(stod(results[15]));
      spacing.push_back(stod(results[16]));
      spacing.push_back(stod(results[17]));
    }
  }
}


void Cube::writecube(const string& filename) const { //what even is the point of this function
  cout << "File-writing has not yet been implemented. " << filename << "\n"; //to rewrite the .vti file in some other crappier format??
}


void Cube::testfunc(){ //what
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
    return coord3d(7,7,7); //someone could write a list of these undocumented errorcodes?
  }
//lnw implementation ported by Jaakko -- the below code puts all the gridpoints in the zerotropic file
//the individual trajs are just lines
/*
  double x = position[0];
  double y = position[1];
  double z = position[2];
  int x0 = int(floor(position[0]));
  int x1 = x0 + 1;
  int y0 = int(floor(position[1]));
  int y1 = y0 + 1;
  int z0 = int(floor(position[2]));
  int z1 = z0 + 1;
  coord3d v000 = coord3d(z0, y0, x0);
  coord3d v001 = coord3d(z0, y0, x1);
  coord3d v010 = coord3d(z0, y1, x0);
  coord3d v011 = coord3d(z0, y1, x1);
  coord3d v100 = coord3d(z1, y0, x0);
  coord3d v101 = coord3d(z1, y0, x1);
  coord3d v110 = coord3d(z1, y1, x0);
  coord3d v111 = coord3d(z1, y1, x1);
  coord3d aux0 = ((double)x1 - x) * v000 + (x - (double)x0) * v001;
  coord3d aux1 = ((double)x1 - x) * v010 + (x - (double)x0) * v011;
  coord3d aux2 = ((double)x1 - x) * v100 + (x - (double)x0) * v101;
  coord3d aux3 = ((double)x1 - x) * v110 + (x - (double)x0) * v111;
  coord3d aux4 = ((double)y1 - y) * aux0 + (y - (double)y0) * aux1;
  coord3d aux5 = ((double)y1 - y) * aux2 + (y - (double)y0) * aux3;
  coord3d res = ((double)z1 - z) * aux4 + (z - (double)z0) * aux5;
  return res;
*/
//lnw implementation - original
//does not compile, gives error
//cube.cc:123:36: error: no match for call to ‘(const Cube) (int&, int&, int&)’
//coord3d v000 = (*this)(z0, y0, x0);
/*
  double x = position[0];
  double y = position[1];
  double z = position[2];
  int x0 = int(floor(position[0]));
  int x1 = x0 + 1;
  int y0 = int(floor(position[1]));
  int y1 = y0 + 1;
  int z0 = int(floor(position[2]));
  int z1 = z0 + 1;
  coord3d v000 = (*this)(z0, y0, x0);
  coord3d v001 = (*this)(z0, y0, x1);
  coord3d v010 = (*this)(z0, y1, x0);
  coord3d v011 = (*this)(z0, y1, x1);
  coord3d v100 = (*this)(z1, y0, x0);
  coord3d v101 = (*this)(z1, y0, x1);
  coord3d v110 = (*this)(z1, y1, x0);
  coord3d v111 = (*this)(z1, y1, x1);
  coord3d aux0 = (x1 - x) * v000 + (x - x0) * v001;
  coord3d aux1 = (x1 - x) * v010 + (x - x0) * v011;
  coord3d aux2 = (x1 - x) * v100 + (x - x0) * v101;
  coord3d aux3 = (x1 - x) * v110 + (x - x0) * v111;
  coord3d aux4 = (y1 - y) * aux0 + (y - y0) * aux1;
  coord3d aux5 = (y1 - y) * aux2 + (y - y0) * aux3;
  coord3d res = (z1 - z) * aux4 + (z - z0) * aux5;
  return res;
*/

//the below pyykko2 implementation gives reasonable, but slightly wrong
/*
  coord3d intpos((int)position[0],(int)position[1],(int)position[2]);
  coord3d sumvec(0,0,0);
  double normsum = 0;
  double norm;
  for (int z=0; z<2; ++z) {
    for (int y=0; y<2; ++y) {
      for (int x=0; x<2; ++x) {
        norm = (coord3d(intpos[0]+x,intpos[1]+y,intpos[2]+z)-position).norm();
        if (norm < 1e-12) { //magic number 1e-12: not checking form norm=0 because of floating point inaccuracy
          return field[position[2]*xrange*yrange+position[1]*xrange+position[0]];
        }
        normsum += 1.0/norm;
        sumvec += field[(intpos[2]+z)*xrange*yrange+(intpos[1]+y)*xrange+intpos[0]+x]/norm;
        }
      }
    }

  return sumvec/normsum;
*/
//method copied from wikipedia https://en.wikipedia.org/wiki/Trilinear_interpolation
//works 
  coord3d intpos((int)position[0],(int)position[1],(int)position[2]);

  double xd = (position[0]-intpos[0]);
  double yd = (position[1]-intpos[1]);
  double zd = (position[2]-intpos[2]);
  
  coord3d c00 = field[intpos[2]*xrange*yrange+intpos[1]*xrange+position[0]] * (1.0 - xd) + field[intpos[2]*xrange*yrange+intpos[1]*xrange+(position[0]+1)] * xd;
  coord3d c01 = field[(intpos[2]+1)*xrange*yrange+intpos[1]*xrange+position[0]] * (1.0 - xd) + field[(intpos[2]+1)*xrange*yrange+intpos[1]*xrange+(position[0]+1)] * xd;
  coord3d c10 = field[intpos[2]*xrange*yrange+(intpos[1]+1)*xrange+position[0]] * (1.0 - xd) + field[intpos[2]*xrange*yrange+(intpos[1]+1)*xrange+(position[0]+1)] * xd;
  coord3d c11 = field[(intpos[2]+1)*xrange*yrange+(intpos[1]+1)*xrange+position[0]] * (1.0 - xd) + field[(intpos[2]+1)*xrange*yrange+(intpos[1]+1)*xrange+(position[0]+1)] * xd;

  coord3d c0 = c00 * (1.0 - yd) + c10 * yd;
  coord3d c1 = c01 * (1.0 - yd) + c11 * yd;

  coord3d c = c0 * (1.0 - zd) + c1 * zd;
  return c;
}

coord3d Cube::getvector3(coord3d position) const{ //skeleton function for tricubic interpolation
  return coord3d(7,7,7);			//later on we figured out that it's probably more expensive to use tricubic interpolation than to make up for the linear one by increasing grid resolution
}

void Cube::splitgrid(string gridfile, string weightfile, int bfielddir) const{

// lnw: could you define/explain what each of those are?
  vector<coord3d> gridpoints; //coordinates from the grid input file are read into this vector
  vector<double> gridweights; //weights from the weight input file are read into this vector
  vector<string> sridpoints; //coordinates from the grid input file are read into this vector
  vector<string> sridweights; //weights from the weight input file are read into this vector
  vector<string> ssopoints;  //coordinates that were classified as diatropic are written into this vector
  vector<string> ssoweights;  //and corresponding weights into this vector
  vector<string> sarapoints; //coordinates that were classified as paratropic are written into this vector
  vector<string> saraweights; //and corresponding weights into this vector
  vector<string> seropoints; //if a coordinate couldn't be classified (trajectory got out of bounds), it is written into this vector
  vector<string> seroweights; //and the corresponding weight into this vector
  vector<string> serointensities; // if a coordinate couldn't be classified, the vector at that coord will be written here.
				// lets one check for convergence

  fstream grid (gridfile);
  string gridline;
  if(!grid.good()) { cout<<"Gridfile '"<<gridfile<<"' was not found.\n"; }
  while (getline (grid, gridline)) {
    istringstream gss(gridline);
    vector<string> gridresults((istream_iterator<string>(gss)),istream_iterator<string>());
    coord3d doublegridresults((stod(gridresults[0])-origin[0])/spacing[0],(stod(gridresults[1])-origin[1])/spacing[1],(stod(gridresults[2])-origin[2])/spacing[2]);
    gridpoints.push_back(doublegridresults); 
    sridpoints.push_back(gridline);
  }

  fstream weights (weightfile);
  string weightsline;
  if(!weights.good()) { cout<<"Weightfile '"<<gridfile<<"' was not found.\n"; }
  while (getline (weights, weightsline)) {
    istringstream wss(weightsline);
    vector<string> weightsresults((istream_iterator<string>(wss)),istream_iterator<string>());
    gridweights.push_back(stod(weightsresults[0])); 
    sridweights.push_back(weightsline);
  }

//we now have the gridpoints in vector<coord3d> gridpoints and the corresponding weights in vector<double> gridweights
//next: get tropicity at each point, then write out two gridfiles

  for(int i=0;i<gridpoints.size();i++){
    trajectory traj(gridpoints[i],getvector(gridpoints[i]),0.01);
    if (i%100==0){cout<<"i="<<i<<"/"<<gridpoints.size()<<"\n";}
        //cout<<"\nNEW TRAJECTORY CREATED AT\t"<<gridpoints[i]<<"\n";
    traj.complete(*this);
    int classification = traj.classify(*this, bfielddir);
    if (classification==-1){
      ssopoints.push_back(sridpoints[i]);
      ssoweights.push_back(sridweights[i]);
    } else if (classification==1){
      sarapoints.push_back(sridpoints[i]);
      saraweights.push_back(sridweights[i]);
    } else if (classification==0){
      seropoints.push_back(sridpoints[i]);
      seroweights.push_back(sridweights[i]);
      ostringstream vectr;
      vectr<<to_string(getvector(gridpoints[i])[0])<<","<<to_string(getvector(gridpoints[i])[2])<<","<<to_string(getvector(gridpoints[i])[2]);
      serointensities.push_back(vectr.str());
    } else {
      seropoints.push_back(sridpoints[i]);
      seroweights.push_back(sridweights[i]);
      ostringstream vectr;
      vectr<<to_string(getvector(gridpoints[i])[0])<<","<<to_string(getvector(gridpoints[i])[1])<<","<<to_string(getvector(gridpoints[i])[2]);
      vectr<<"\t@\t"<<sridpoints[i];
      serointensities.push_back(vectr.str());
      //cout<<"couldn't classify this point :o(\n"; // lnw: you mean, could neither classify nor not classify this point? // jaakko: see trajectory.cc line 263: in case a trajectory is completed but its curvature is zero, trajectory::classify returns 2.
	//jaakko: this happens for example when the length of a vector is so close to zero that the length between to points is zero at double precision
    }
  }
//now write dia, para and zero points and weights to respective files
  ofstream diapout, diawout, parapout, parawout, zeropout, zerowout, zeroint;
  ostringstream diapoutfile;
  diapoutfile << gridfile << "-diatropic";
  diapout.open(diapoutfile.str());
  for (int i=0;i<ssopoints.size();i++) {
    diapout<<ssopoints[i]<<"\n";
  }
  diapout.close();
  ostringstream diawoutfile;
  diawoutfile << weightfile << "-diatropic";
  diawout.open(diawoutfile.str());
  for (int i=0;i<ssoweights.size();i++) {
    diawout<<ssoweights[i]<<"\n";
  }
  diawout.close();

  ostringstream parapoutfile;
  parapoutfile << gridfile << "-paratropic";
  parapout.open(parapoutfile.str());
  for (int i=0;i<sarapoints.size();i++) {
    parapout<<sarapoints[i]<<"\n";
  }
  parapout.close();
  ostringstream parawoutfile;
  parawoutfile << weightfile << "-paratropic";
  parawout.open(parawoutfile.str());
  for (int i=0;i<saraweights.size();i++) {
    parawout<<saraweights[i]<<"\n";  
  }
  parawout.close();

  ostringstream zeropoutfile;
  zeropoutfile << gridfile << "-zerotropic";
  zeropout.open(zeropoutfile.str());
  for (int i=0;i<seropoints.size();i++) {
    zeropout<<seropoints[i]<<"\n";  
  }
  zeropout.close();
  ostringstream zerowoutfile;
  zerowoutfile << weightfile << "-zerotropic";
  zerowout.open(zerowoutfile.str());
  for (int i=0;i<seroweights.size();i++) {
    zerowout<<seroweights[i]<<"\n";  
  }
  zerowout.close();
  zeroint.open("zerointensities.txt");
  for (int i=0;i<serointensities.size();i++){
    zeroint<<serointensities[i]<<"\n";
  }
  
}


vector<vector<int>> Cube::gettropplane(string filename, int bfielddir, int fixeddir, double fixedcoord) const {
  double steplength = 0.01;
  vector<vector<int>> tropplane;
  if (fixeddir==2) {
  fixedcoord = (fixedcoord-origin[1])/spacing[1];
  /// fixedcoord should probably be scaled (according to the .vti header (the gimic outputfile spacing)) at the very first line of this function!
    for (int y=0;y<yrange;y++) {
    cout<<"y = "<<y<<"/"<<yrange<<endl;
    vector<int> point_tropicity;
    tropplane.push_back(point_tropicity);
      for (int x=0;x<xrange;x++){
        trajectory traj(coord3d(x,y,fixedcoord),getvector(coord3d(x,y,fixedcoord)),steplength);
        traj.complete(*this);
        tropplane[y].push_back(traj.classify(*this, bfielddir));
      }
    }
  return tropplane;
  }
 
  else if (fixeddir==1) {
    for (int z=0;z<zrange;z++) {
    cout<<"z = "<<z<<"/"<<zrange<<endl;
    vector<int> point_tropicity;
    tropplane.push_back(point_tropicity);
      for (int x=0;x<xrange;x++){
        trajectory traj(coord3d(x,fixedcoord,z),getvector(coord3d(x,fixedcoord,z)),steplength);
        traj.complete(*this);
        tropplane[z].push_back(traj.classify(*this, bfielddir));
      }
    }
  return tropplane;
  }

  else if (fixeddir==0) {
    for (int y=0;y<yrange;y++) {
    cout<<"y = "<<y<<"/"<<yrange<<endl;
    vector<int> point_tropicity;
    tropplane.push_back(point_tropicity);
      for (int z=0;z<zrange;z++){
        trajectory traj(coord3d(fixedcoord,y,z),getvector(coord3d(fixedcoord,y,z)),steplength);
        traj.complete(*this);
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
 

