/*******************************************************************
*  get_distance_4.cpp
*  June 7, 2006
*  MMFuller
*
*  Derived from get_sitedistance_4.cpp
*
*  Purpose: Calculate straight line distance between two sites based on 
*           their latitude and longitude. Print list of sites whose
*	    distance is less than some target value. Uses formula that
*	    accounts for curvature of earth (not great circle distance).         
*
*           Allows user to refine search by setting limits on longitude values.
*           Requests name for input file.
*           Longitude limit zones are now pre-set (switch statement).
*           Prints the above member variables where appropriate.
*           Replaced generic list1, list2 with Bird_Data, GSL_list
*           Loops now expect different data for different input files.
*           No longer tracks min/max longitude values for each list.
*           Generic wrt input filename (prompts for filename).
*           Defaults to GSL_long weather station input file.
*	    
*
*  New Features
*           Altered for BBS data which lacks year, bph, and bpm 
*           Bird Data input file format:
*           First Line: integer for file size
*           Data Lines (order & type of data):
              string: Name 
              double: Latitude, Longitude, (expects decimal degrees) 
*
*
*  Required Files:
*  GSL_long
*
*
******************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

struct site {
  double lat, lon;
  string name;
  int longitude_zone;
  int elevation;
  int frostfree_days;
};

double distance_fromStruct(site a, site b);
double distance_fromCoord(double x1, double x2, double y1, double y2);
double distance_greatcircle(double x1, double x2, double y1, double y2);
void STOP();


int main()
{
  ifstream fin1;
  ifstream fin2;
  ofstream fout;


  int i,j;
  int Bird_Data_size;
  int GSL_list_size;
  int datacount(0);
  int zone;       //longitude zone
  double target;  //desired maximum distance between 2 sites
  double actual_distance;
  double X1,X2,Y1,Y2;
  double long_min, long_max;  //min and max values for longitude
  char ans;      //holds user input to yes/no question?
  string Bird_Data;
  string GSL_list = "GSL_long";
  string outfile;
  string testsite1, testsite2;
 
  site* Bird_Data_sites;
  site* GSL_list_sites;
  
  cout << "\nEnter Bird_Data: ";
  cin >> Bird_Data;
  cout << "\nEnter outfile name: ";
  cin >> outfile;
  cout << "\nEnter target distance in miles (double): " << flush;
  cin >> target;

  fout.open(outfile.c_str());

  fout << "Distance betw Sites (GSL and CBC)\n"
       << "target distance = " << target << " miles\n"
       << "Elev Code -1 = missing data\n"
       << "FF-days Code 9999 = no. of days > 365\n"
       << "Zone\tBird_Data\tGSL_List\tBBS Lat1\tBBS Long1\tGSL Lat2\tGSL Long2"
       << "\tDistance\tElev\tFF-days\n";
  
  fin1.open(Bird_Data.c_str());
  if(fin1.fail()){
    cout << "open failure for file: " << Bird_Data << endl;
    exit(1);
  } 

  fin1 >> Bird_Data_size;
  Bird_Data_sites = new site[Bird_Data_size];
  for(i = 0; i < Bird_Data_size; i++){
     fin1 >> Bird_Data_sites[i].name 
          >> Bird_Data_sites[i].lat 
          >> Bird_Data_sites[i].lon;
  }
  fin1.close();
  
  fin2.open(GSL_list.c_str());
  if(fin2.fail()){
    cout << "open failure for file: " << GSL_list << endl;
    exit(1);
  } 

  fin2 >> GSL_list_size;
  GSL_list_sites = new site[GSL_list_size];
  for(i = 0; i < GSL_list_size; i++){
    fin2 >> GSL_list_sites[i].name
         >> GSL_list_sites[i].lat 
         >> GSL_list_sites[i].lon
         >> GSL_list_sites[i].frostfree_days 
         >> GSL_list_sites[i].elevation;
  }
  fin2.close();

  cout << "\nSet limits on longitude? ";
  cin >> ans;
  if(ans == 'y' || ans == 'Y') {
    cout << "\nAVAILABLE LONGITUDE ZONES\n-----------------------\n"
	 << "\n1 = 80.625 - 82.375"
	 << "\n2 = 94.0 - 96.2"
	 << "\n3 = 97.375 - 99.125"
	 << "\n4 = 109.125 - 110.875"
	 << "\n5 = 120.125 - 121.875\n"
	 << "\nEnter Zone: " << flush;
    cin >> zone;

    switch(zone)
    {
      case 1:
          long_min = 80.625;
          long_max = 82.375;
          break;
      case 2:
          long_min = 94.0;
          long_max = 96.2;
          break;
      case 3:
          long_min = 97.375;
          long_max = 99.125;
          break;
      case 4:
          long_min = 109.125;
          long_max = 110.875;
          break;
      case 5:
          long_min = 120.125;
          long_max = 121.875;
          break;
      default:
          cout << "\nUnknown Zone\n";
          exit(1);
    }
  }

  cout << "\nmin = " << long_min << "\nmax = " << long_max << flush;

  //calculate distance between GSL and CBC sites
  if(ans == 'y' || ans == 'Y') {
    for(i = 0; i < Bird_Data_size; i++) {
      for(j = 0; j < GSL_list_size; j++) {
	X1 = Bird_Data_sites[i].lat;
	X2 = GSL_list_sites[j].lat;
	Y1 = Bird_Data_sites[i].lon;
	Y2 = GSL_list_sites[j].lon;
		  
	//calculate distances constrained to longitude limits
	if(Y1 >= long_min && Y1 <= long_max 
	   && Y2 >= long_min && Y2 <= long_max) {

	  actual_distance = distance_greatcircle(X1,X2,Y1,Y2);
	  if(actual_distance <= target) {
	    fout << zone << "\t"
                 << Bird_Data_sites[i].name << "\t" << GSL_list_sites[j].name << "\t"
		 << Bird_Data_sites[i].lat << "\t" << Bird_Data_sites[i].lon << "\t"
		 << GSL_list_sites[j].lat << "\t" << GSL_list_sites[j].lon << "\t"
		 << actual_distance << "\t" << GSL_list_sites[j].elevation << "\t"
		 << GSL_list_sites[j].frostfree_days << endl; 
	    datacount++;
	  }
	}
      }
    }
  }
  
  else {
    for(i = 0; i < Bird_Data_size; i++) {
      for(j = 0; j < GSL_list_size; j++) {
	X1 = Bird_Data_sites[i].lat;
	X2 = GSL_list_sites[j].lat;
	Y1 = Bird_Data_sites[i].lon;
	Y2 = GSL_list_sites[j].lon;
			  
	//calculate distances with no constraint
	actual_distance = distance_greatcircle(X1,X2,Y1,Y2);

        //if(actual_distance == 0) STOP();

	if(actual_distance <= target) {
	  fout << Bird_Data_sites[i].name << "\t"
	       << GSL_list_sites[j].name << "\t"
	       << Bird_Data_sites[i].lat << "\t" << Bird_Data_sites[i].lon << "\t"
	       << GSL_list_sites[j].lat << "\t" << GSL_list_sites[j].lon << "\t"
		 << actual_distance << "\t" << GSL_list_sites[j].elevation << "\t"
		 << GSL_list_sites[j].frostfree_days <<  endl;
	  datacount++;
	}
      }
    }
  }
  fout.close();

  cout << "\nNumber of matching sites: " << datacount << endl;
  
  delete [] Bird_Data_sites;
  delete [] GSL_list_sites;
  
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

void STOP()
{
	char ans;
	cout << "STOP? ";
	cin >> ans;
	if(ans == 'y') exit(1);
}
