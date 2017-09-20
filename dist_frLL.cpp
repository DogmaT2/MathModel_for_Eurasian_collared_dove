/*******************************************************************
*  dist_frLatLon.cpp
*  July 31, 2006
*  MMFuller
* 
* Calculates straight line distance between 2 points based on their
* latitude-longitude coordinates in decimal degrees. Distance is
* calculated using great-circle distance formula.
*
* Input file 1: list of origin coordinates
* Input file 2: list of destination coordinates
*
* Output file: list of distances in km.
********************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

struct coords {
  double lat, lon;
};

double distance_greatcircle(double x1, double x2, double y1, double y2);

int main()
{
  ifstream fin;
  ofstream fout;

  string file1;
  string file2;
  string outfile;

  int i,j;
  int f1_size;
  int f2_size;
  
  coords* f1_coords;
  coords* f2_coords;

  cout << "\nEnter File 1: ";
  cin >> file1;
  cout << "\nEnter File 2: ";
  cin >> file2;
  cout << "\nEnter outfile: ";
  cin >> outfile;

  fout.open(outfile.c_str());

  fin.open(file1.c_str());
  if(fin.fail()){
    cout << "open failure for file: " << file1 << endl;
    exit(1);
  } 

  fin1 >> f1_size;
  f1_coords = new coords[f1_size];
  for(i = 0; i < f1_size; i++) {
     fin >> f1_coords[i].lat >> f1_coords[i].lon;
  }
  fin.close();

  fin.open(file2.c_str());
  if(fin.fail()){
    cout << "open failure for file: " << file2 << endl;
    exit(1);
  } 

  fin >> f2_size;
  if(f1_size != f2_size) {
     cout << "\nERROR: file sizes don't match!";
     exit(1);
  }

  f2_coords = new coords[f1_size];
  for(i = 0; i < f1_size; i++) {
     fin >> f2_coords[i].lat >> f2_coords[i].lon;
  }
  fin.close();

   for(i = 0; i < f1_size; i++) {
	X1 = f1_coords[i].lat;
	X2 = f2_coords[i].lat;
	Y1 = f1_coords[i].lon;
	Y2 = f2_coords[i].lon;
			  
	//calculate distances with no constraint
	fout << actual_distance = distance_greatcircle(X1,X2,Y1,Y2) << endl;
  }
  fout.close();
  delete [] f1_coords;
  delete [] f2_coords;
  cout << "\nActions Completed\n";
  
  return 0;
}
/**************************************************************/

double distance_greatcircle(double x1, double x2, double y1, double y2)
{
	//divisions by 57.2958 needed to convert degress to radians
	//earth radius in km is 6378.7, in miles is 3963.0
	
	double earthradius = 3963.0;
	long double temp;
	
	temp = earthradius * (acos( (sin(x1/57.2958) * sin(x2/57.2958))
								+ (cos(x1/57.2958) * cos(x2/57.2958)) 
								* cos(y2/57.2958 - y1/57.2958) ));
    return temp;
}
