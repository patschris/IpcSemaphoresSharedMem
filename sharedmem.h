#ifndef _sharedmem_
#define _sharedmem_

Shared_memory* getSharedMemory(void);
Matches* getMatches(void);
void detachSharedMems(Shared_memory *, Matches *);

#endif