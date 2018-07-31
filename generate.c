#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * The parent process generates N new process using fork system call	
 */
void generateProcesses(int N){
	for (int i=0;i<N;i++) {
		pid_t pid = fork();
		if (pid<0) {
			perror("Fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) { 
			break;
		}
	}
}