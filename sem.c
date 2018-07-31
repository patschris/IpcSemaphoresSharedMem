#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*
 * Create and initialize all the necessary for the programs semaphores
 */
void createSemaphores (int *seminread, int *seminwrite, int *semoutread, int *semoutwrite, int *semmatches) {
	if ((*seminread = semget(1821,1,IPC_CREAT |0666)) == -1 || (*seminwrite = semget(1453,1,IPC_CREAT |0666)) == -1 || 
		(*semoutread = semget(1940,1,IPC_CREAT |0666)) == -1 || (*semoutwrite = semget(1789,1,IPC_CREAT |0666)) == -1 || 
		(*semmatches = semget(2016,1,IPC_CREAT |0666)) == -1) 
	{
		perror("Cannot get semaphores");
		exit(EXIT_FAILURE);
	}
	semctl(*seminread,0,SETVAL,0);
	semctl(*seminwrite,0,SETVAL,1);
	semctl(*semoutread,0,SETVAL,0);
	semctl(*semoutwrite,0,SETVAL,1);
	semctl(*semmatches,0,SETVAL,1);

}

/*
 * Delete the semaphores that created in the function createSemaphores
 */
void deleteSemaphores (int seminread,int seminwrite, int semoutread, int semoutwrite, int semmatches) {
	semctl(seminread,0,IPC_RMID,0);
	semctl(seminwrite,0,IPC_RMID,0);
	semctl(semoutread,0,IPC_RMID,0);
	semctl(semoutwrite,0,IPC_RMID,0);
	semctl(semmatches,0,IPC_RMID,0);
}

/* 
 * Acquire the semaphore given as argument
 */
void downSem (int sem) {
	struct sembuf semopr;
	semopr.sem_num = 0;
	semopr.sem_flg = 0;
	semopr.sem_op = -1;
	semop(sem, &semopr, 1);
}

/* 
 * Release the semaphore given as argument
 */
void upSem (int sem) {
	struct sembuf semopr;
	semopr.sem_num = 0;
	semopr.sem_flg = 0;
	semopr.sem_op = 1;
	semop(sem, &semopr, 1);
}