#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "generate.h"

/*
 * Create a shared memory (type Shared_memory) in order to send and receive messsages
 */
Shared_memory* getSharedMemory (void) {
	int shm_id;
	Shared_memory* mymem;
	if((shm_id=shmget((key_t) getpid()-1,sizeof(Shared_memory),0666 | IPC_CREAT))==-1){
		perror("Shmget shared memory");
    	exit (EXIT_FAILURE);
	}
	mymem = (Shared_memory*) shmat((int)shm_id, (char *) 0, 0);
	memset(mymem->inds.row,0,1024);
	mymem->inds.pid=-1;
	memset(mymem->outds.row,0,1024);
	mymem->outds.pid=-1;
	return mymem;
}

/*
 * Create a shared memory (type Matches) in order to count the matches (the same process read and wrote a line)
 */
Matches* getMatches (void) {
	int shm_id;
	Matches* match;
	if ((shm_id=shmget((key_t) getpid()-10,sizeof(Matches),0666 | IPC_CREAT))==-1) {
		perror("Shmget matces");
		exit (EXIT_FAILURE);
	}
	match = (Matches*) shmat((int)shm_id, (char *) 0, 0);
	match->counter=0;
	return match;
}

/*
 * Detach the shared memories
 */
void detachSharedMems (Shared_memory *mymem, Matches *match){
	shmdt(mymem);
	shmdt(match);
}