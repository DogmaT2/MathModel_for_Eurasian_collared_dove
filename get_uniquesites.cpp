/******
get_uniquesites.cpp
May 4, 2006
MMFuller

Purpose: extract single line of site data from file containing
multiple lines for each site name.

*********/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

int main()
{
  ifstream fin1;
  ifstream fin2;
  ofstream fout;

  int namecount(0), datacount(1);
  int namelistsize;
  int datalistsize;
  double coordinate1, coordinate2;
  //string namelist = "testnames";
  //string datafile = "testsites";
  string namelist = "CBC_sitenames.txt";
  string datafile = "Site_latlong.txt";
  string outfile = "CBC_coord.out";
  string testname;
  string nextname;
  
  fin1.open(namelist.c_str());
  fin2.open(datafile.c_str());
  fout.open(outfile.c_str());

  fout << "CBC EUCD Sites and Lat-Long Coordinates\n"
       << "ABBREV\tLat\tLong\n";

  fin1 >> namelistsize;
  fin2 >> datalistsize;
  fin2 >> nextname >> coordinate1 >> coordinate2;

  //compare with names and get coordinates
  while(namecount < namelistsize) {
    fin1 >> testname;          
    fout << testname << "\t";     

    if(nextname == testname){
      fout << coordinate1 << "\t" << coordinate2 << endl;
    }
    //move through data list to next new name
    while(nextname == testname && datacount < datalistsize) {
      fin2 >> nextname >> coordinate1 >> coordinate2;
      datacount++;
    } 

    namecount++;
  }

  fin1.close();
  fin2.close();
  fout.close();
  
  cout << "\nActions Completed\n";
  return 0;
}



  
