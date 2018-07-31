#ifndef _generate_
#define _generate_

/* 
 * A shared memory to store a message
 */
typedef struct {
	pid_t pid;		// who wrote the message
	char row[1024];	// message
} Message;

/* 
 * A share memory to send and receive a message
 */
typedef struct {
	Message inds;	// P writes here, C processes read from here
	Message outds;	// C processes write here, P reads from here
} Shared_memory;

/* 
 * A shared memory to count the matches (the same process read and wrote a line)
 */
typedef struct {
	int counter;
} Matches;

void generateProcesses(int);

#endif