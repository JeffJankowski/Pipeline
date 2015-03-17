#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <pthread.h>
#include <queue>
#include <sstream>

using namespace std;

//mutex to buffer
pthread_mutex_t buffer_mutex;
//buffer abstracted as queue
queue<char> buf;
bool cont;

/**
 * Will take character from buffer, modify it, and send it
 * to stdout.
 * Should be run in a separate thread
 */
static void* filter(void *dptr) {
	char ch;
	
	//loop while main thread hasn't exitted OR
	// buffer is NOT empty
	while (cont || !buf.empty()) {
		//if the buffer is empty, just poll
		if (buf.empty())
			continue;
		
		//get next char in buffer queue
		pthread_mutex_lock(&buffer_mutex);
		ch = buf.front();
		buf.pop();
		pthread_mutex_unlock(&buffer_mutex);
		
		//format stderr logging
		string sz;
		stringstream out;
		out << buf.size();
		sz = out.str();
		if (isalpha(ch))
			cerr << "Filter:  Modifying '" << ch << "', removed from BUFFER (" << sz << ") and sent to SINK" << endl;
		else
			cerr << "Filter:  Ignoring non-alpha, removed from BUFFER (" << sz << ") and sent to SINK" << endl;
		
		
		//FILTER STEP
		if (isupper(ch))
			ch = tolower(ch);
		else if (islower(ch))
			ch = toupper(ch);
		
		//send char to std out
		cout.put(ch);
		flush(cout);
		
		//sleep for 35ms
		usleep(35000);
	}
	
	//exit the thread
	pthread_exit(NULL);
	return 0;
}

/**
 * Reads characters in from stdin, and sends them to 
 * the buffer for filtering by the filter thread.
 * 
 * Usage: filter
 */
int main (int argc, char * const argv[]) {
	char ch;
	pthread_t filter_thread;
	cont = true;
	
	//initialize buffer mutex
	pthread_mutex_init(&buffer_mutex, NULL);
	//create and start the filter thread
	pthread_create(&filter_thread, NULL, filter, NULL);
	
	//get char from stdin until EOF
	while (cin.get(ch)) {
		//get mutex and add char to buffer
		pthread_mutex_lock(&buffer_mutex);
		buf.push(ch);
		pthread_mutex_unlock(&buffer_mutex);
	}
	
	//indicate that stdin is "done"
	cont = false;
	//join our filter thread
	pthread_join(filter_thread, NULL);	
	return 0;
}

