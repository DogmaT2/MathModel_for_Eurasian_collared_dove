/*******************************************************************
*  Count_nBBSinZonList.cpp
*  Dec 19, 2006
*  MMFuller
*
*  Derived from get_nBBSinZonList.cpp

*
*  Purpose: Counts number of BBS sites that occur within a given
*           longitudinal range. Allows use of multiple longitude ranges
*           or set of zones whose width is a constant interval. Accounts for
*           whether longitude values are negative or positive.
*
*           Requests name for input file.
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
const double LONGABSMAX = 119.30;

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
  int count(0);
  int Bird_Data_size, zonefile_size;
  int zone;       //longitude zone identifier
  double X1,Y1;
  double long_interval;  //zone width value
  double long_min, long_max;  //min and max values for longitude
  double max_longinterval = LONGABSMAX - LONGABSMIN; 
  double zoneshift; //optional value for shifting zones east or west
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
  cout << "\nUse zone file?";
  cin >> ans;

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
  fout << "\nResults for file: " << Bird_Data << endl
       << "Sites Present in Zone\n"
       << "Zone/Interval\tLonMin\tLonMax\tCount\n";

  if(ans == 'y') {
	 cout << "\nEnter zone data file name: ";
	 cin >> zonefile;
  	 fin.open(zonefile.c_str());
  	 if(fin.fail()){
    	 cout << "open failure for file: " << zonefile << endl; exit(1); } 
  	 else {  
   		fin >> zonefile_size;
 		zonedata = new site[zonefile_size];
		//Uses site member variables lat for Longmin, lon for Longmax
		//Assumes data are either all positive or all negative doubles
 		for(i = 0; i < zonefile_size; i++){
   			fin >> zonedata[i].lat 
        		    >> zonedata[i].lon;
  		}
		fin.close();
		//make sure sign of longitude values is positive
		if(zonedata[0].lon < 0)
			for(i = 0; i < zonefile_size; i++) {
				zonedata[i].lon *= -1;
			}
	 	}
  }
  else {
	
	cout << "\nEnter zone interval ";
	cin >> long_interval;
	cout << "\nEnter zone shift value: ";
	cin >> zoneshift;
	zonefile_size = floor(max_longinterval/long_interval);
	zonedata = new site[zonefile_size];
	zonedata[0].lat = LONGABSMIN + zoneshift;
	zonedata[0].lon = LONGABSMIN + zoneshift + long_interval;
	//use of i=1 is intentional
	for(i = 1; i < zonefile_size; i++) {
		zonedata[i].lat = zonedata[i-1].lon;
		zonedata[i].lon = zonedata[i-1].lon + long_interval;
	}
  }

  //count sites located within zones
  for(i = 0; i < zonefile_size; i++) {
  	count = 0;
  	zone = i+1;
  	long_min = zonedata[i].lat;
	long_max = zonedata[i].lon;
  	for(j = 0; j < Bird_Data_size; j++) {
		Y1 = Bird_Data_sites[j].lon;
		if(Y1 >= long_min && Y1 < long_max) count++;
	}
	fout << zone << "\t" << long_min << "\t"
	     << long_max << "\t" << count << endl;
  }

  fout.close();
  
  delete [] Bird_Data_sites;
  delete [] zonedata;

  cout << "\nEnd Count_nBBSinZone\n";
  
  return 0;
}
/**************************************************************/
