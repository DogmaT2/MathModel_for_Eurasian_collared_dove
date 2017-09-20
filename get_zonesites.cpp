/*******************************************************************
*  get_zonesites.cpp
*  June 12, 2006
*  MMFuller
*
*  Derived from get_sitedistance_4.cpp
*  Updated December 22, 2006: added new zone (AL-MI)
*
*  Purpose: Generates a list of BBS sites that occur within a given
*           longitudinal range.
*
*           Requests name for input file.
*           Longitude limit zones are  pre-set (switch statement).
*           Member variables of struct "site":
*           * Year
*           * longitude_zone
*           * elevation
*           * frostfree_days
*           * bird_count1
*           * bird_count2
*	    
*
*  FILE FORMAT
* 
*           Bird Data input file format:
*           First Line: integer for file size
*           Data Lines (order & type of data):
              string: Name 
              double: Latitude, Longitude, 
              int: Year, 
              double: Birds per Hour, Birds per Mile
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
  int year;
  int longitude_zone;
  double birds_perhour;
  double birds_permile;
};

int main()
{
  ifstream fin1;
  ifstream fin2;
  ofstream fout;
  
  fout.setf(ios::fixed);
  fout.setf(ios::showpoint);
  fout.precision(4);

  int i,j;
  int Bird_Data_size;
  int zone;       //longitude zone
  double X1,Y1;
  double long_min, long_max;  //min and max values for longitude
  string Bird_Data;
  string outfile;
  site* Bird_Data_sites;
  
  cout << "\nEnter Datafile: ";
  cin >> Bird_Data;
  cout << "\nEnter outfile name: ";
  cin >> outfile;

  fout.open(outfile.c_str());

  fout << "Sites Present in Zone\n"
       << "Zone\tSite\tLat1\tLong1\tBPH\tBPM\tYear\n";
  
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
          >> Bird_Data_sites[i].lon
	 >> Bird_Data_sites[i].year 
          >> Bird_Data_sites[i].birds_perhour 
          >> Bird_Data_sites[i].birds_permile;
  }
  fin1.close();
  
  cout << "\nSet limits on longitude."
     << "\nAVAILABLE LONGITUDE ZONES\n-----------------------\n"
	 << "\n1 = 80.625 - 82.375"
	 << "\n3 = 89.625 - 91.375"
	 << "\n4 = 97.375 - 99.125"
	 << "\n5 = 109.125 - 110.875"
	 << "\n6 = 120.125 - 121.875\n"
	 << "\n7 = 95.0 - 96.175\n"     
	 << "\nEnter Zone: " << flush;
    cin >> zone;

    switch(zone)
    {
      case 1:
          long_min = 80.625;
          long_max = 82.375;
          break;
      case 2:
	cout << "\nZone 2 not available!"
	     << "\nSetting zone to Zone 3";
          long_min = 89.625;
          long_max = 91.375;
          break;
      case 3:
          long_min = 89.625;
          long_max = 91.375;
          break;
      case 4:
          long_min = 97.375;
          long_max = 99.125;
          break;
      case 5:
          long_min = 109.125;
          long_max = 110.875;
          break;
      case 6:
          long_min = 120.125;
          long_max = 121.875;
          break;
	  case 7:
          long_min = 95.0;
          long_max = 96.175;
          break;
      default:
          cout << "\nUnknown Zone\n";
          exit(1);
    }
  

  cout << "\nmin = " << long_min << "\nmax = " << long_max << flush;

  //determine whether site located within zone
	for(i = 0; i < Bird_Data_size; i++) {
		X1 = Bird_Data_sites[i].lat;
		Y1 = Bird_Data_sites[i].lon;
		
		//calculate distances constrained to longitude limits
		if(Y1 >= long_min && Y1 <= long_max) {
			fout << zone << "\t"
			<< Bird_Data_sites[i].name << "\t"
			<< Bird_Data_sites[i].lat << "\t" << Bird_Data_sites[i].lon << "\t"
			<< Bird_Data_sites[i].birds_perhour << "\t"
			<< Bird_Data_sites[i].birds_permile << "\t"
			<< Bird_Data_sites[i].year << "\n";
		}
	}
  
  fout.close();
  
  delete [] Bird_Data_sites;
  
  cout << "\nEnd get_zonesites\n";
  
  return 0;
}
/**************************************************************/
