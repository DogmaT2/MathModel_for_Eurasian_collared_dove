/****************************************************************************
 *  lat-long_dist_frbase.cpp
 *  Created by MMFuller December 21, 2006
 *  Copyright 2006. All rights reserved.
 *  Derived from lat-long_distance.cpp
 *
 *  Calculates the distance (miles or km) between a particular latitude,
 *  and a list of sites based on their latitude-longitude coordinates.
 *  Calculates distance using great-circle distance. For each calculation
 *  the longitude of the base is set to same longitude of each site.
 *  
 *  All calcaulations use lat-long in decimal degrees.
 * 
 *  Takes data from command line or file. 
 *  INPUT FILE FORMAT
 *  	int file length (number of records)
 *  	//for each record (one record per line)
 *  	double latitude   double longitude
 *  	//where data are in decimal degress
 *
 *  GREAT-CIRCLE (miles): uses formula for distances on a sphere:
 *    3963.0 * arccos[sin(lat1/57.2958)*sin(lat2/57.2958) + 
 *	           (cos(lat1/57.2958)*cos(lat2/57.2958)
 *				*cos(lon2/57.2958-lon1/57.2958))]
 *    where 3963 is the radius of the earth in miles.
 *
 ****************************************************************************/

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

//distance functions in miles and km
double distance_greatcircleMI(double x1, double x2, double y1, double y2);
double distance_greatcircleKM(double x1, double x2, double y1, double y2);

int main()
{
	ifstream fin;
	ofstream fout;
	
	int i,j;
	int listsize, units;
	double lat1, lat2, lon1, lon2;
	double X1, X2, Y1, Y2;
	double actual_distance;
	double baselat;
	char ans = 'n';      //holds user input to yes/no question?
	string inputfile = "testdist.dat";
	string outfile = "LL_distfrBase.out";
	site* sitelist;
	
	cout << "\nCalculate distance in miles (1) or km (2)? ";
	cin >> units;
	cout <<"\nEnter file mode? ";
	cin >> ans;
	if(ans == 'n') {
		ans = 'y';
		while(ans == 'y') {
			cout << "\nEnter lat1, lon1, lat2, lon2" << endl;
			cin >> lat1 >> lon1 >> lat2 >> lon2;
			if(units == 1) {
				cout << "Distance = " 
				<< distance_greatcircleMI(lat1,lat2,lon1,lon2);
			}

			else if(units == 2) {
				cout << "Distance = " 
				<< distance_greatcircleKM(lat1,lat2,lon1,lon2);
			}
			cout << "\nAgain? ";
			cin >> ans;
			cout << endl;
		}
	}
		
	else {
		cout << "\nEnter output file: ";
		cin >> outfile;
		cout << "\nEnter base latitude: ";
		cin >> X1;
		cout << "\nUse default input file? ";
		cin >> ans;
		if(ans == 'n' || ans == 'N') {
			cout << "\nEnter file name: ";
			cin >> inputfile;
		}
		
		fout.open(outfile.c_str());
		fin.open(inputfile.c_str());
		if(fin.fail()){
			cout << "open failure for file: " << inputfile << endl;
			exit(1);
		}
		fout 	<< "\nDistance from a baseline latitude to points north"
			<< "\nData File = " << inputfile
			<< "\nBaseline latitude = " << X1 << endl << endl
			<< "Lat\tLon\tDist\n";
		fin >> listsize;
		sitelist = new site[listsize];
		for(i = 0; i < listsize; i++){
			fin >> sitelist[i].lat
			>> sitelist[i].lon;
		}
		fin.close();
		for(i = 0; i < listsize; i++) {
				X2 = sitelist[i].lat;
				Y1 = sitelist[i].lon;
				Y2 = sitelist[i].lon;
				
				//calculate distances
				if(units == 1) actual_distance = distance_greatcircleMI(X1,X2,Y1,Y2);
				else if(units == 2) actual_distance = distance_greatcircleKM(X1,X2,Y1,Y2);
				fout << sitelist[i].lat << "\t"
					<< sitelist[i].lon << "\t"
					<< actual_distance << endl;
		}
	}
	fout.close();
	delete [] sitelist;

	cout << "Calculations Completed: lat-long_dist_frbase\n";
	return 0;
	
}
/**************************************************************************/

double distance_greatcircleMI(double x1, double x2, double y1, double y2)
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


double distance_greatcircleKM(double x1, double x2, double y1, double y2)
{
	//divisions by 57.2958 needed to convert degress to radians
	//earth radius in km is 6378.7, in km is 6378.8	
	double earthradius = 6378.8;
	long double temp;
	
	temp = earthradius * (acos( (sin(x1/57.2958) * sin(x2/57.2958))
					   + (cos(x1/57.2958) * cos(x2/57.2958)) 
					   * cos(y2/57.2958 - y1/57.2958) ));
    return temp;
}


