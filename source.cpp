#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Reads provided text file, character by character, and flushes 
 * each individually to stdout. 
 * 
 * Usage: source in.txt
 */
int main (int argc, char* argv[]) {
	//file path
	char* file;
	char ch;
	//input stream to file
	ifstream is;
	
	//seed rand()
	srand(NULL);
	file = argv[1];
	is.open(file);
	//if file doesn't exist
	if (!is) {
		cerr << "cannot open input file: " << file << endl;
		return 1;
	}
	
	//gets each character from file until EOF
	while (is.get(ch)) {
		//send char to stdout
		cout.put(ch);
		flush(cout);
		
		if (isalpha(ch))
			cerr << "Source:  Read '" << ch << "' from \"" << file << "\" and sent to FILTER" << endl;
		else
			cerr << "Source:  Read non-alpha from \"" << file << "\" and sent to FILTER" << endl;
		
		//sleep from somewhere between 10-49ms
		usleep((rand() % 40 + 10) * 1000);
	}

	//close file stream
	is.close();
	return 0;
}