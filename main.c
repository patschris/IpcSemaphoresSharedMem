#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "generate.h"
#include "sharedmem.h"
#include "sem.h"

int main(int argc , char * argv[]) {
	FILE * fp;
	size_t len;
	ssize_t read;
	char *line = NULL;
	int N, K, seminread, seminwrite, semoutread, semoutwrite, semmatches, pid_mom = getpid(), nlines=0;
	if (argc == 3) {
		N = atoi(argv[1]);
		K = atoi(argv[2]);
	}
	else {
		printf("Invalid number of arguments\nExecution command: ./myprog N K\nN: The amount of P-Processes to be created\nK: Number of iterations\n");
		exit(EXIT_FAILURE);
	}
	Shared_memory *mymem = getSharedMemory();
	Matches *match = getMatches();
	createSemaphores(&seminread,&seminwrite,&semoutread, &semoutwrite,&semmatches); // create all the necessary semaphores
	srand(time(NULL));
	if ((fp=fopen("doc.txt","r")) == NULL)	exit(EXIT_FAILURE); // open the document that we are going to read
	while ((read = getline(&line, &len, fp)) != -1) {
		fgets(line,1024,fp);
		nlines++; //number of lines
	}
	fclose(fp);
	generateProcesses(N); // generate N child processes

	if(getpid()!=pid_mom) { //P process (N processes - children)
		pid_t P_pid_match=0;
		char tempString[1024];
		pid_t tempPid;
		memset(tempString,0,1024);
		while (1) {

			// read a random line from the file and write it to the shared memory
			if ((fp=fopen("doc.txt","r")) == NULL){
				perror("fopen failed");				
				exit(EXIT_FAILURE);
			}
			int randline=rand()%nlines;
			fseek(fp,0,SEEK_SET);
			for (int j=0;!feof(fp) && j<randline;j++) fgets(line,1024,fp);
			fclose(fp);
			
			// read from the shared memory what a process wrote	
			downSem(seminwrite);	
			mymem->inds.pid=getpid();
			strcpy(mymem->inds.row,line);
			upSem(seminread);
			downSem(semoutread);
			tempPid=mymem->outds.pid;
			strcpy(tempString,mymem->outds.row);
			
			// write the total matches after STOP message is received		
			if (tempPid==-2) {
				upSem(semoutread);
				upSem(seminwrite);
				downSem(semmatches);
				match->counter=match->counter+P_pid_match;
				upSem(semmatches);
				break;		
			}			
			else {
				// We have a match (the same process read and wrote the same message)
				if (tempPid==getpid()) {
					P_pid_match++;											
					printf("P process %d read from shared memory %d : %s", (int)getpid(),tempPid,tempString);		
				}
				mymem->outds.pid=-1;
				memset(mymem->outds.row,0,1024);
				upSem(semoutwrite);
			}
		}	
	}
	else { //C process (Only 1)
		int rc;
		pid_t temppid, exitpid;
		char tempstring[1024];
		for (int i=0;i<K;i++) {
		
			// read from the shared memory what the parent process wrote
			downSem(seminread);
			temppid=mymem->inds.pid;
			strcpy(tempstring,mymem->inds.row);
			mymem->inds.pid=-1;
			memset(mymem->inds.row,0,1024);
			upSem(seminwrite);

			// convert the message to have only capital letters
			for (int j=0;j<strlen(tempstring);j++) tempstring[j]=toupper(tempstring[j]);

			// write the converted message
			downSem(semoutwrite);
			mymem->outds.pid = temppid;
			strcpy(mymem->outds.row,tempstring);	
			memset(tempstring,0,1024);
			temppid=-1;
			upSem(semoutread);			
		}
		
		// Finished the iteration. Send STOP message.
		downSem(semoutwrite);
		mymem->outds.pid = -2;
		strcpy(mymem->outds.row,"STOP");
		upSem(semoutread);				
		
		// Wait children processes to exit
		for (int i=0;i<N;i++) {
			exitpid = waitpid (-1,&rc,0);
			if (rc < 0) printf("exit failure %d\n", (int)exitpid);		
		}
		printf("\nThe number of P processes is %d\nThe number of iterations is %d\nTotal: %d matches out of %d\n",N, K,  match->counter, K);
		
		// Dismiss shared memories and semaphores
		detachSharedMems(mymem, match);
		deleteSemaphores(seminread,seminwrite, semoutread, semoutwrite, semmatches);
	}
	return 0;
}