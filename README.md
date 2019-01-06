The program simply simulates a Process Scheduling of an Operating system. Each process has its own priority. 
The code orders the processes according to their priorities and arrival times.
The execution is based on the Round Robin algorithm
The situation of the ready queue is printed at every change in the queue, as new processes arrives, as a process terminates

The code is heavily commented, so there is no need to extra information about the code. In
general “scheduling process” is executed through the main class. Reading the definition file is executed
in main too. I used a writer method to write to the output file, used ComparePriorities method which are
explained in detail as comments to the code. There is also another class which is “Process” file, where all
the instances of a process is kept, also reading the necessary information for the process is held here
(such as reading the code file, arrival time, priority etc.)

Compile at ubuntu terminal: g++ -o <execfile> *.cpp
How to run at ubuntu terminal : ./execname or
                                ./execname definition.txt output.txt
