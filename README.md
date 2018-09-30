# IPC Semaphores and Shared Memory

# Desciption
The purpose of this program is to show how Inter-Process Communication(IPC) - especially [shared memory](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm)- 
and [semaphores](https://cse.yeditepe.edu.tr/~sbaydere/fall2010/cse331/files/SystemVIPC.pdf) work. There are two types of processes: P and 
C. There is 1 C and *N* (command line parameter) P processes. C and P processes communicate via a shared memory that contains two 
structures for bidirectional communication: in-ds (P → C) and out-ds (C → P).
<br/>
![projectScheme](https://github.com/patschris/IpcSemaphoresSharedMem/blob/master/scheme.PNG)<br/>

Written in C, Ubuntu 16.04.3 LTS. 

## Execution command
./myprog *N* *K*<br/>
where<br/>
*N*: The amount of P-Processes to be created<br/>
*K*: Number of iterations

## Makefile
- `make` to compile
- `make clean` to the delete object files and the executable

## References
[1] https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm<br/>
[2] https://cse.yeditepe.edu.tr/~sbaydere/fall2010/cse331/files/SystemVIPC.pdf
