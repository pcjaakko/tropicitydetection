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

	coord3d getvector(vector<int> coord){
		int x = coord[0];
		int y = coord[1];
		int z = coord[2];
		double doublex = field[z*(field.size()/3.0)+y*(field.size()/9.0)+x][0];
		double doubley = field[z*(field.size()/3.0)+y*(field.size()/9.0)+x][1];
		double doublez =  field[z*(field.size()/3.0)+y*(field.size()/9.0)+x][2];
		coord3d flow(doublex,doubley,doublez);
		return flow;
	}
		
};

int main () {
	Cube cube("jvec-minimal.vti");
	cout << "Let's do a dot product:   cube[0] . cube[1] = " << cube.field[0].dot(cube.field[1]) << "\n";
	cout << "Let's do a cross product: cube[0] x cube[1] = " << cube.field[0].cross(cube.field[1]) << "\n";
	cube.writecube("file.txt");
	cout << "getting first vector from cube:     " << cube.getvector({0,0,0})<<"\n";
	cout << "getting vector at (1,1,2) from cube:" << cube.getvector({1,1,2})<<"\n";
	return 0;
	//cout << "Dot product between the first and the second vector is: " << coord3d::cube.field[0].dot(cube.field[1]) << "\n";
}
