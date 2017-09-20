/*
 *  cleanfile.cpp
 *  
 *  Created by Fuller on 11/20/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 *  Reformats CBC data from csv to tsv, line by line. 

   Variables in CBC File:
   sitename  abbrv  lat  lon  subnat  yr  spcd  obs  obsfw
   hrs  mitot  birdcnt  sptot  birdptyhr  birdsperhour

   Variables sometimes missing (NAN) are: obsfw, sptot
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

int WORDSIZE = 35;

int main()
{
	ifstream fin;
	ofstream fout;
	
	int i, j, linecount(0), wordcount(1), filesize;
	int linesize = 80;
	char data[25], savefile[25];
	char oneline[linesize];
 	char word[WORDSIZE];
	
	cout << "\nEnter data file: ";
	cin >> data;
	cout << "\nEnter save file: ";
	cin >> savefile;
	
	fin.open(data);	
	fout.open(savefile);
	
	fin >> filesize;
	fout << filesize;
	while(linecount < filesize && !fin.eof()) {
		//get site name and preserve comma
		fin.getline(word, WORDSIZE, ',');
		fout << word << ",\t";
		
		//get rest of data and remove commas
		while(wordcount < 15) {
			fin.getline(word, WORDSIZE, ',');
			fout << word << "\t";
			wordcount++;
		}
		wordcount = 1;
		linecount++;
		//fout << endl;
	}
		
	fin.close();
	fout.close();
	cout << "\nAll files closed\n";
	return 0;
}

