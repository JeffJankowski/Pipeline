#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Reads stdin, character by character, and writes each
 * individually to the provided text file.
 * 
 * Usage: sink out.txt
 */
int main (int argc, char* argv[]) {
	//file path
	char* file;
	char ch;
	
	//seed rand()
	srand(NULL);
	file = argv[1];
	//create new file, or delete and recreate
	ofstream os(file);
	
	//get char from stdin until EOF
	while (cin.get(ch)) {
		//write character to file
		os.put(ch);
		
		if (isalpha(ch))
			cerr << "Sink:    Recevied '" << ch << "' from FILTER and written to \"" << file << "\"" << endl;
		else
			cerr << "Sink:    Recevied non-alpha from FILTER and written to \"" << file << "\"" << endl;
		
		//sleep from somewhere between 20-59ms
		usleep((rand() % 40 + 20) * 1000);
	}

	//close the file stream
	os.close();
	return 0;
}