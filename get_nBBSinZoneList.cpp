/*******************************************************************
*  get_nBBSinZonList.cpp
*  Dec 19, 2006
*  MMFuller
*
*  Derived from get_zonesites.cpp
*
*  Purpose: Generates a list of BBS sites that occur within a given
*           longitudinal range. Allows use of multiple longitude ranges
*           or set of zones whose width is a constant interval. Accounts for
*           whether longitude values are negative or positive.
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
  		
	   Zone Data input file
	   size
	   Longmin Longmax
*
*
******************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;
const double LONGABSMIN = 75.95;
cont double LONGABSMAX = 119.30;

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
  ifstream fin;
  ofstream fout;
  
  fout.setf(ios::fixed);
  fout.setf(ios::showpoint);
  fout.precision(4);

  int i,j;
  int Bird_Data_size, zonefile_size;
  int zone;       //longitude zone identifier
  double X1,Y1;
  double long_interval;  //zone width value
  double long_min, long_max;  //min and max values for longitude
  double max_longinterval = LONGABSMAX - LONGABSMIN; 
  int zoneshift; //optional value for shifting zones east or west
  char ans;
  string Bird_Data;
  string outfile;
  string zonefile; //holds list of longitude values
  site* Bird_Data_sites;
  site* zonedata;
  
  cout << "\nEnter BBS Datafile: ";
  cin >> Bird_Data;
  cout << "\nEnter outfile name: ";
  cin >> outfile;
  fout.open(outfile.c_str());

  //Get BBS data
  fin.open(Bird_Data.c_str());
  if(fin.fail()){
    cout << "open failure for file: " << Bird_Data << endl;
    exit(1); } 
  else {  
   	fin >> Bird_Data_size;
 	Bird_Data_sites = new site[Bird_Data_size];
 	for(i = 0; i < Bird_Data_size; i++){
   		fin >> Bird_Data_sites[i].name 
         	>> Bird_Data_sites[i].lat 
        	>> Bird_Data_sites[i].lon
	 	>> Bird_Data_sites[i].year 
         	>> Bird_Data_sites[i].birds_perhour 
          	>> Bird_Data_sites[i].birds_permile;
  	}
 	fin.close();
  }
  //print headings of output file
  fout << "Sites Present in Zone\n"
       << "Zone/Interval\tSite\tLat1\tLong1\tBPH\tBPM\tYear\n";

  cout << "\nUse zone file? ;
  cin >> ans;
  if(ans == 'y') {
	 cout << "\nEnter zone data file name: ";
	 cin >> zonefile;
  	 fin.open(zonefile.c_str());
  	 if(fin.fail()){
    	 cout << "open failure for file: " << zonefile << endl; exit(1); } 
  	 else {  
   		fin >> zonefile_size;
 		zonedata = new site[zonefile_size];
		//Uses lat for Longmin, lon for Longmax
		//Assumes data are either all positive or all negative doubles
 		for(i = 0; i < zonefile_size; i++){
   			fin >> zonedata[i].lat 
        		    >> zonedata[i].lon
  		}
		fin.close();
		//make sure sign of longitude values is positive
		if(zondata[0].lon < 0)
			for(i = 0; i < zonefile_size; i++) {
				zonedata[i].lon *= -1;
			}
	 	}
  }
  else {
	
	cout << "\nEnter zone interval ";
	cin >> long_interal;
	cout << "\nEnter zone shift value: ";
	cin >> zoneshift;
	zonefile_size = floor(max_longinterval/long_interval);
	zonedata = new site(zonefile_size];
	zonedata[0].lat = LONGABSMIN + zoneshift;
	zonedata[0].lon = LONGABSMIN + zoneshift + long_interval;
	//use of i=1 is intentional
	for(i = 1; i < zonefile_size; i++) {
		zonedata[i].lat = zonedata[i-1].lon;
		zonedata[i].lon = zonedata[i-1].lon + long_interval;
	}
  }

  //determine whether site located within zones
  for(i = 0; i < zonefile_size; i++) {
  	long_min = zonedata[i].lat;
	long_max = zonedata[i].lon;
  	for(j = 0; j < Bird_Data_size; j++) {
		X1 = Bird_Data_sites[i].lat;
		Y1 = Bird_Data_sites[i].lon;
		
		//calculate distances constrained to longitude limits
		if(Y1 >= long_min && Y1 <= long_max) {
			fout << zone << "\t"
			<< Bird_Data_sites[i].name << "\t"
			<< Bird_Data_sites[i].lat << "\t" 
			<< Bird_Data_sites[i].lon << "\t"
			<< Bird_Data_sites[i].birds_perhour << "\t"
			<< Bird_Data_sites[i].birds_permile << "\t"
			<< Bird_Data_sites[i].year << "\n";
		}
	}
  }


  fout.close();
  
  delete [] Bird_Data_sites;
  
  cout << "\nEnd get_zonesites\n";
  
  return 0;
}
/**************************************************************/
