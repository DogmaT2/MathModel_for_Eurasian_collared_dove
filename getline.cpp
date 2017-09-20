#include <iostream>
#include <fstream>
#include <cstdlib>
#include "/usr/local/MFstdlib.cpp"

using namespace std;

int WORDSIZE = 15;

int main()
{
	ifstream fin;
	ofstream fout;
	string data;
	char ans = 'y';
	char word[WORDSIZE];
	
	cout << "\nEnter data file: ";
	cin >> data;
	open_infile(fin, data);
	
	while(ans = 'y' && !fin.eof()) {
		fin.getline(word, WORDSIZE, ',');
		cout << endl << word << "\nGet next word? ";
		cin >> ans;
	}
	
	cout << "\nThat's All\n";
	return 0;
}