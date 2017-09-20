/**********************************
parseCBCdat2.cpp
Date: Nov 20, 2005 
Michael M Fuller

Parses data from the Christmas Bird Count for a given species.
Here the given specie is the Eurasian collared-dove. 
Extracts data for this species from database. Data are in 
the form of comma separated fields.

Expects integer for filesize at top of input file.
Expects input file to have a comma after the site name, no
commas after other data. Data separated by tabs.
(In other words, expects format created by program: cleanfile

Data Format:

SQL> select NAME ||','||
  2  ABBREV ||','||
  3  LATITUDE ||','||
  4  LONGITUDE ||','||
  5  SUBNATIONAL1_CODE||','||
  6  COUNT_YR ||','||
  7  SPECIES_CODE  ||','||
  8  NUM_OBSERVERS ||','||
  9  OBSERVERS_FW ||','||
 10  DURATION_HRS  ||','||
 11  PARTY_MILES_TOT  ||','||
 12  HOW_MANY   ||','||
 13  SPECIES_TOT  ||','||
 14  BIRDS_PARTY_H  ||','||
 15  BIRDS_PARTY_MI

**********************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

int WORDSIZE = 25;
// filesize for all data = 54218; //number of lines in file
// dovesize for all data = 1458; //number of lines containing eucdov in CBC data file

int main()
{
	ifstream fin;
	ofstream fout;

	int i, count(0), dovcount(0), linecount(0), wordcount(0), 
            datacount(0), filesize, dovesize(1000);
	int name_column = 7;
	int linesize = 80;
	int yr, obs, obsfw, birdcnt, sptot;
	int output_flag = 1;
	double lat, lon, mitot, hrs, birdptyhr, birdspermi;
	char sitename[35];
	string abbrv;
	string junk;
	char subnat[7];
	char spcd[7];
	char data[25], savefile[25];
	char ans = 'y';
	char testword[20]="eucdov";
	char oneline[linesize];
 	char word[WORDSIZE];
	
	cout << "\nEnter data file: ";
	cin >> data;
	cout << "\nEnter save file: ";
	cin >> savefile;

	fin.open(data);	
	fout.open(savefile);
	int namelist[dovesize];

	fout << "abbrv\tyr\tlat\tlon\tbirdspermi\n";
		
	//sitename  abbrv  lat  lon  subnat  yr  spcd  obs  obsfw
	//hrs  mitot  birdcnt  sptot  birdptyhr  birdspermi
		
	//fout << "sitename\tabbrv\tlat\tlon\tsubnat\tyr\tspcd 
	//	\tobs\tobsfw\thrs\tmitot\tbirdcnt\tsptot\tbirdptyhr\tbirdspermi\n";
		
	fin >> filesize;

 	//record line numbers of lines that have eucdov
	while(linecount < filesize && !fin.eof()) {
		fin.getline(word, WORDSIZE, ',');
		wordcount = 1;
		while(wordcount < name_column) {
			fin >> word;
			//cout << "\nword = " << word;
			wordcount++;
		}
		//cout << "\nword2 = " << word;
		if(!strcmp(word,testword)) {
			namelist[dovcount] = linecount;
			dovcount++; 
		}
		//get rest of line
		fin.getline(oneline, linesize); 
		linecount++;
	}

	fin.close();

		cout << "\nNumber of lines with eucdov: " << dovcount;

	cout << "\nData lines containing " << testword << endl;
	for(i = 0; i < dovcount; i++) {
		cout << namelist[i] << endl << flush;
 	}

	fin.open(data);	
	fin >> filesize;
	linecount = 0;
	datacount = 0;
	
	cout << "count\tline\tyr\tlat\tlon\tbirdspermi\n";

	while(linecount < filesize && !fin.eof()) {
		//cout << "\nlinecount " << linecount 
		//<< "\nnamelist[i] " << namelist[datacount];
		if(linecount == namelist[datacount]) {
			//get sitename
			fin.getline(word, WORDSIZE, ',');
			//cout << "\nword " << word;
			strcpy(sitename,word);
			//cout << "\nsitename" << sitename;
			//get rest of data line
			fin >> abbrv >> lat >> lon >> subnat >> yr >> spcd >> obs;
			//get obsfw field, which may be an int or NAN
			fin >> word;
                        if(isdigit(word))
                            obsfw = atoi(word);
			else
			   output_flag = 0;
			fin >> hrs >> mitot >> birdcnt;
			//get sptot field, which may be int or NAN
                        fin >> word;
			if(isdigit(word))
			   sptot = atoi(word);
                        else
			   output_flag = 0;
                        fin >> birdptyhr >> birdspermi;
			
			//cout << "\nData\n"
			//	<< abbrv << " " << lat << " " << lon << " " << subnat << " "
			//	<< yr << " " << spcd << " " << obs << " " << obsfw << endl
			//	<< hrs << " " << mitot << " " << birdcnt << " " << sptot 
			//	<< " " << birdptyhr << " " << birdspermi << endl;
			
			cout << endl << datacount+1 << "\t" << namelist[datacount] << "\t"
				<< yr << "\t" << lat 
				<< "\t" << lon << "\t" << birdspermi << endl;
			
			fout << abbrv << "\t" << yr << "\t" << lat 
				<< "\t" << lon << "\t" << birdspermi << endl;
			
			//get rest of line
			//fin.getline(oneline, linesize);  
			datacount++;                  
		}
		else {
			fin.getline(word, WORDSIZE, ',');
			//cout << "\nword else " << word;
			for(i = 0; i < 14; i++) {
				fin >> junk;
			}
			//cout << "\nlast junk = " << junk;
		}
		linecount++;
	}
	
	fin.close();	
	fout.close();
	cout << "\nThanks for the gumball!\n";
	return 0;
}
