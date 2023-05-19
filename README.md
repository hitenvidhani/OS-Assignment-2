## Matrix Multiplication with Parallelism
This repository contains a C program that performs matrix multiplication using parallelism provided by the Linux process and threads libraries. The program consists of three parts: P1, P2, and the scheduler program S.

# P1 - Reading and Timing
P1 is a C program that takes three filenames (in1.txt, in2.txt, and out.txt) as command line arguments. It reads two matrices of arbitrary size from in1.txt and in2.txt, which satisfy the criteria for matrix multiplication. The sizes of the matrices are passed as command line arguments.

P1 spawns multiple threads, varying from 1 to arbitrarily large, to read different parts of the matrices concurrently. The program records the time taken to read the entire file into memory with different numbers of threads, measured at the granularity of nanoseconds. The timing results are then plotted against the number of threads for different input sizes.

To compile and run P1:

csharp
Copy code
gcc P1.c -o P1 -pthread
./P1 i j k in1.txt in2.txt out.txt
P2 - Computing and Timing
P2 is a C program that uses IPC mechanisms to receive the rows and columns read by P1. Similar to P1, P2 spawns multiple threads, varying from 1 to arbitrarily large, to compute the cells in the product matrix. The program stores the product matrix in the file out.txt.

P2 also records the time taken to compute the product with different numbers of threads, measured at the granularity of nanoseconds. The timing results are then plotted against the number of threads for different input sizes.

To compile and run P2:

Copy code
gcc P2.c -o P2 -pthread
./P2
S - Scheduler Program
S is a scheduler program that simulates a uniprocessor scheduler. It spawns two child processes that execute P1 and P2, respectively. S uses different scheduling algorithms to manage the execution of the processes:

Round Robin with time quantum 2 ms
Round Robin with time quantum 1 ms
Completely Fair Scheduler where the vruntime is updated every 1 ms
The scheduler program records the total turnaround time and waiting time for different workload sizes. The results are then plotted against the workload size, comparing the two scheduling algorithms. The switching overhead in each case is also analyzed.

To compile and run S:

bash
Copy code
gcc scheduler.c -o scheduler
./scheduler
Usage
To run the entire assignment:

csharp
Copy code
./groupX_assignment2.out i j k in1.txt in2.txt out.txt
Replace i, j, and k with the appropriate dimensions for the matrices.
The matrix data for in1.txt and in2.txt should be provided according to the specified dimensions.
The resulting product matrix will be stored in out.txt.
Please refer to the included sample input, output, and Python script for generating and multiplying matrices.

