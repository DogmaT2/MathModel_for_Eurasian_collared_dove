/******
get_sitedistance.cpp
May 4, 2006
MMFuller

Purpose: Calculate straight line distance between two sites
         based on their latitude and longitude. Print list 
	 of sites whose distance is less than some target 
	 value. 

*********/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

struct site {
	double lat, lon;
	string name;
};

double distance_fromStruct(site a, site b);
double distance_fromCoord(double x1, double x2, double y1, double y2);

int main()
{
  ifstream fin;
  ofstream fout;

  int i,j;
  int list1_size;
  int list2_size;
  double target;  //desired maximum distance between 2 sites
  double actual_distance;
  double X1,X2,Y1,Y2;
  string list1;
  string list2;
  string outfile = "distance-list.out";
 
  site* list1_sites;
  site* list2_sites;
  
  fout.open(outfile.c_str());

  cout << "\nEnter list1: ";
  cin >> list1;
  cout << "\nEnter list2: ";
  cin >> list2;
  cout << "\nEnter target distance in miles (double): " << flush;
  cin >> target;

  cout << "\nPrinting header" << flush;
  fout << "Distance betw Sites (GSL and CBC)\n"
       << "target distance = " << target << " miles" << endl
       << "List1\tList2\tLat-Long1\tLat-Long2\tDistance\n" << flush;
  
  cout << "\nGetting list 1" << flush;
  fin.open(list1.c_str());
  if(fin.fail()){
    cout << "open failure for file: " << list1 << endl;
    exit(1);
  } 
  fin >> list1_size;
  list1_sites = new site[list1_size];
  for(i = 0; i < list1_size; i++){
	  fin >> list1_sites[i].name >> list1_sites[i].lat >> list1_sites[i].lon;
  }
  fin.close();
  
  cout << "\nGetting list 2" << flush;
  fin.open(list2.c_str());
  if(fin.fail()){
    cout << "open failure for file: " << list2 << endl;
    exit(1);
  } 
  fin >> list2_size;
  list2_sites = new site[list2_size];
  for(i = 0; i < list2_size; i++){
	  fin >> list2_sites[i].name >> list2_sites[i].lat >> list2_sites[i].lon;
  }
  fin.close();

  //compare sites 
  for(i = 0; i < list1_size; i++) {
	  for(j = 0; j < list2_size; j++) {
	    // actual_distance = distance(list1_sites[i], list2_sites[j]); 
            X1 = list1_sites[i].lat;
            X2 = list2_sites[j].lat;
            Y1 = list1_sites[i].lon;
            Y2 = list2_sites[j].lon;
            actual_distance = distance_fromCoord(X1,X2,Y1,Y2);
		  if(actual_distance <= target) {
			  fout << list1_sites[i].name << " "
			       << list2_sites[j].name << " "
			       << list1_sites[i].lat << "-" << list1_sites[i].lon << " "
			       << list2_sites[i].lat << "-" << list2_sites[i].lon << " "
			       << actual_distance << endl;
		  }
	  }
  }

  fout.close();
  
  delete [] list1_sites;
  delete [] list2_sites;
  
  cout << "\nActions Completed\n";
  
  return 0;
}
/**************************************************************/
double distance_fromStruct(site a, site b) 
{
  double x1 = a.lat;
  double x2 = b.lat;
  double y1 = a.lon;
  double y2 = b.lon;

  double t1 = (x1-x2)*69.1;
  double t2 = (y1-y2)*69.1 * cos(x2/57.3); 

  double temp = (t1* t1)+(t2*t2);  
   
  return sqrt(temp);
}

double distance_fromCoord(double x1, double x2, double y1, double y2)
{
   //each degree in latitude = approx 69.1 miles
   //distance of a degree in longitude depends on latitude because of curvature of earth
   //values given below correct for curvature

   double t1 = (x1-x2)*69.1;
   double t2 = (y1-y2)*69.1 * cos(x2/57.3); 

   double temp = (t1* t1)+(t2*t2);  
     
   return sqrt(temp);
}
