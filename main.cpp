#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//ready and write constants
#define RD		0
#define WR		1

int src_to_fltr[2], fltr_to_snk[2];
int pidSrc, pidFltr, pidSnk;
char *fileIN, *fileOUT;

/*
 * Example of source-filter-sink pipeline.
 * Will copy the given input file to output file path, and
 * the uppercase characters will be lowercase, and vice versa.
 *
 * Executables called in new processes: source, filter, sink
 * Usage: pipeline in.txt out.txt
 */
int main (int argc, char* argv[]) {
	
	//check for file arguments
	if (argc != 3) {
		printf("insufficient arguments:  pipeline in.txt out.txt\n");
		return 1;
	} else {
		fileIN = argv[1];
		fileOUT = argv[2];
	}
	
	//setup the pipes
	pipe(src_to_fltr);
	pipe(fltr_to_snk);
	
	/* Create source-filter-sink BACKWARDS! It may be slower, but
	   we don't want data to sit in the stream buffer before the 
	   filter is set up */
	
	//Setup sink
	// read from fltr_to_snk pipe
	// write to stdout
	if (pidSnk = fork() == 0) {
		close(src_to_fltr[RD]);
		close(src_to_fltr[WR]);
		
		close (fltr_to_snk[WR]);
        dup2(fltr_to_snk[RD], RD);
		close(fltr_to_snk[RD]);
		
		execl("sink", "sink", fileOUT, (char*)0);
		exit(1);
	}
	
	//Setup filter
	// read from src_to_fltr pipe
	// write to fltr_to_snk pipe
	if (pidFltr = fork() == 0) {
		
		close (src_to_fltr[WR]);
        dup2(src_to_fltr[RD], RD);
		close(src_to_fltr[RD]);
		
		
		close (fltr_to_snk[RD]);
        dup2(fltr_to_snk[WR], WR);
		close(fltr_to_snk[WR]);
		
		execl("filter", "filter", (char*)0);
		exit(1);
	}
	
	//Setup sink
	// read from stdin
	// write to src_to_fltr pipe
	if (pidSrc = fork() == 0) {
		close(fltr_to_snk[RD]);
		close(fltr_to_snk[WR]);
		
		close(src_to_fltr[RD]);
		dup2(src_to_fltr[WR], WR);
		close(src_to_fltr[WR]);
		
		execl("source", "source", fileIN, (char*)0);
		exit(1);
	}
	
	//PARENT:
	//close all the pipes
	close(src_to_fltr[RD]);
	close(src_to_fltr[WR]);
	close(fltr_to_snk[RD]);
	close(fltr_to_snk[WR]);
	
	//wait for source to stop sending data and exit
	waitpid(pidSrc, NULL, 0);
	//wait for filter to finish modifying the data
	waitpid(pidFltr, NULL, 0);
	//wait for the sink to finishing writing to output
	waitpid(pidSnk, NULL, 0);
	
	return 0;
}
