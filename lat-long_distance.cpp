/****************************************************************************
 *  lat-long_distance.cpp
 *  Created by MMFuller May 7, 2006
 *  Copyright 2006. All rights reserved.
 *
 *  Calculates the distance (miles or km) in between two sites based on their
 *  latitude-longitude coordinates. Calculates distance in 
 *  three ways: basic, advanced, and great-circle distance.
 *  All calcaulations use lat-long in decimal degrees.
 * 
 *  Takes data from command line or file. 
 *
 *  MODIFIED: 3 DEC 2006
 *  Added functions for distance in km.
 *  Added if statements and user prompt for choice of units (miles or km)
 *
 *  BASIC (miles): distance based on miles per degree lat-long:
 *   sqrt(x^2 + y^2) 
 *   where: x = 69.1*(lat1-lat2), y = 53.0*(lon1-lon2)
 *
 *  ADVANCED (miles): includes corrections for bias due to earth's curvature:
 *   sqrt(x^2 + y^2) 
 *   where: x = 69.1*(lat1-lat2), y = 69.1*(lon1-lon2)*cos(lat1/57.2958)
 *   where division by 57.2958 converts decimal latitude to radians.
 *            
 *  GREAT-CIRCLE (miles): uses formula for distances on a sphere:
 *    3963.0 * arccos[sin(lat1/57.2958)*sin(lat2/57.2958) + 
 *	           (cos(lat1/57.2958)*cos(lat2/57.2958)
 *				*cos(lon2/57.2958-lon1/57.2958))]
 *    where 3963 is the radius of the earth in miles.
 *
 *  Great-circle distance is the most accurate but the other
 *  two methods are useful over short distances. 
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
double distance_basicMI(double x1, double x2, double y1, double y2);
double distance_advancedMI(double x1, double x2, double y1, double y2);
double distance_greatcircleMI(double x1, double x2, double y1, double y2);
double distance_basicKM(double x1, double x2, double y1, double y2);
double distance_advancedKM(double x1, double x2, double y1, double y2);
double distance_greatcircleKM(double x1, double x2, double y1, double y2);

//output functions
void Dist_Miles(double la1, double la2, double lo1, double lo2);
void Dist_Km(double la1, double la2, double lo1, double lo2);

int main()
{
	ifstream fin;
	ofstream fout;
	
	int i,j;
	int listsize, units;
	double lat1, lat2, lon1, lon2;
	double X1, X2, Y1, Y2;
	double actual_distance;
	char ans = 'n';      //holds user input to yes/no question?
	string inputfile = "testdist.dat";
	string outfile = "testdist.out";
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
			if(units == 1) Dist_Miles(lat1, lat2, lon1, lon2);
			else if(units == 2) Dist_Km(lat1, lat2, lon1, lon2);
			cout << "\nAgain? ";
			cin >> ans;
			cout << endl;
		}
	}
		
	else {
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
		fin >> listsize;
		sitelist = new site[listsize];
		for(i = 0; i < listsize; i++){
			fin >> sitelist[i].name 
			>> sitelist[i].lat 
			>> sitelist[i].lon;
		}
		fin.close();
		for(i = 0; i < listsize/2; i++) {
			for(j = listsize/2; j < listsize; j++) {
				X1 = sitelist[i].lat;
				X2 = sitelist[j].lat;
				Y1 = sitelist[i].lon;
				Y2 = sitelist[j].lon;
				
				//calculate distances
				if(units == 1) actual_distance = distance_greatcircleMI(X1,X2,Y1,Y2);
				else if(units == 2) actual_distance = distance_greatcircleKM(X1,X2,Y1,Y2);
				fout << sitelist[i].name << " "
					<< sitelist[j].name << " "
					<< sitelist[i].lat << "-" << sitelist[i].lon << " "
					<< sitelist[j].lat << "-" << sitelist[j].lon << " "
					<< actual_distance << endl;
					
				cout << sitelist[i].name << "-"
					<< sitelist[j].name << "="
					<< actual_distance << endl;
			}
		}
	}
	fout.close();

	cout << "Finished\n";
	return 0;
	
}
/**************************************************************************/

double distance_basicMI(double x1, double x2, double y1, double y2)
{
	//each degree in latitude = approx 69.1 miles; longitude = approx 53 miles.
	
	double t1 = (x1-x2)*69.1;
	double t2 = (y1-y2)*53.0; 
	double temp = (t1*t1)+(t2*t2);  
	
	return sqrt(temp);
	
}

double distance_advancedMI(double x1, double x2, double y1, double y2)
{
	//each degree in latitude = approx 69.1 miles
	//distance of a degree in longitude depends on latitude because of curvature of earth
	//values given below correct for curvature
	//divisions by 57.2958 needed to convert degress to radians
	
	double t1 = (x2-x1)*69.1;
	double t2 = (y2-y1)*69.1 * cos(x1/57.2958); 
	double temp = (t1*t1)+(t2*t2);  
	
	return sqrt(temp);
}


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


double distance_basicKM(double x1, double x2, double y1, double y2)
{
	//each degree in latitude = approx 111.2 miles; longitude = approx 85.3 miles.
	
	double t1 = (x1-x2)*111.2;
	double t2 = (y1-y2)*85.3; 
	double temp = (t1*t1)+(t2*t2);  
	
	return sqrt(temp);
	
}

double distance_advancedKM(double x1, double x2, double y1, double y2)
{
	//each degree in latitude = approx 111.2 km
	//distance of a degree in longitude depends on latitude because of curvature of earth
	//values given below correct for curvature
	//divisions by 57.2958 needed to convert degress to radians
	
	double t1 = (x2-x1)*111.2;
	double t2 = (y2-y1)*111.2 * cos(x1/57.2958); 
	double temp = (t1*t1)+(t2*t2);  
	
	return sqrt(temp);
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


void Dist_Miles(double la1, double la2, double lo1, double lo2) {

   cout << "\nBasic: " 
	<< distance_basicMI(la1, la2, lo1, lo2) << " miles"
	<< "\nAdvanced: " 
	<< distance_advancedMI(la1, la2, lo1, lo2) << " miles"
	<< "\nGreat Circle: " 
	<< distance_greatcircleMI(la1, la2, lo1, lo2) << " miles";
}

void Dist_Km(double la1, double la2, double lo1, double lo2) {

   cout << "\nBasic: " 
	<< distance_basicKM(la1, la2, lo1, lo2) << " km"
	<< "\nAdvanced: " 
	<< distance_advancedKM(la1, la2, lo1, lo2) << " km"
	<< "\nGreat Circle: " 
	<< distance_greatcircleKM(la1, la2, lo1, lo2) << " km";
}
	
