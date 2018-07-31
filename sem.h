#ifndef _sem_
#define _sem_

void createSemaphores(int* ,int*, int*, int*,int*);
void deleteSemaphores(int ,int, int, int, int);
void downSem(int);
void upSem(int);

#endif
