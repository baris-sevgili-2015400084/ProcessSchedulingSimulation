#include <iostream>
#include <algorithm>
#include <string.h>
#include <queue>
#include <functional>
#include <cstdio>
#include <fstream>
#include "Process.h"

using namespace std;

struct ComparePriorities   // a struct that compares priorities, if equal, arrival times of the functions
{
    bool operator()(const Process &a, const Process &b) {

        if (a.priority !=b.priority)
            return a.priority > b.priority;     //lower value means higher priority
        else  return a.arTime > b.arTime;       //FIFO manner
    }
};

/*A function that writes the current situation of the ready queue(priority queue) to the output file
 * outFile: output file
 * curTime: current time
 * priority queue: ready queue
 */
void OutputWriter(string outFile, int curTime, priority_queue<Process, vector<Process>, ComparePriorities> pqProcesses){

    ofstream out(outFile,ios::out | ios::app);
    out << curTime << ":HEAD-";
    if(pqProcesses.empty())
        out<< "-";
    while(!pqProcesses.empty()){        //prints all the processes' current situation in the ready queue
        Process p1 = pqProcesses.top();
        pqProcesses.pop();
        int instrCount=p1.instrPtr+1;   //counter, denotes next instruction of the related process
        out<< p1.procName<<"["<< instrCount<<"]-";
    }
    out <<  "TAIL\n";
    out.close();
}

/* I've executed the necessary scheduling operations in the main function
 *
 */
int main(int args, char* argv[]) {

    if(args==1){ //sets definition file and output file in case of args given

        argv[1]= (char*)"./definition.txt";
        argv[2]= (char*)"./output.txt";

    }

    ifstream defnInput(argv[1], ios::in);   // defines input file as argv[1]
    string _procName;   //process name
    string _codeFile;   //name of the code file
    int _arrivalTime;   //arrival time of the process
    int _priority;      //priority of the process

    vector<Process> procVec;    //a vector which holds all of the processes
    int counter = 0;            //counter that determines the process ID(arrival line# of the process)
    while (defnInput.good()){   //reads until the end of the input file
        defnInput >> _procName >> _priority >> _codeFile >> _arrivalTime;  //each token assigned to related instances

        if(!(procVec.size()>0&&_procName==procVec.back().procName)) { //handles the end of line exception
            Process temp=Process(_priority, _arrivalTime, _procName, _codeFile, counter);   //creates process object
            procVec.push_back(temp);    //pushes the created process to the processes vector
            counter++;
        }
    }
    defnInput.close();

    vector <int> burstTimes(procVec.size(),0);  //initializing a vector for burst times of processes


    /*
     * calculates burst times of processes, by adding each instructions'
     * execution times, which are defined in the instruction vectors of the processes,
     * in the process object beforehand
     */

    for (int j = 0; j <procVec.size() ; j++) {
        for (int i = 0; i <procVec[j].instrVec.size() ; ++i) {
            burstTimes[j]+=(procVec[j].instrVec[i]);
        }
    }

    int procVecSize=procVec.size(); //integer holding the size of the processes vector

    /*
     *
     *  Scheduling process takes place from now on
     *
     */
    int currTime=0;     //current time which will be set to a new value after each operation, if necessary
    int procPtr=0;      //a pointer to keep at which process we are at

    /* priority queue which orders the processes in it according to the ComparePriorities function
     * which is the "ready queue",
     */
    priority_queue<Process, vector<Process>, ComparePriorities> pqProcesses;

    OutputWriter(argv[2],currTime,pqProcesses); //prints the initial situation of the ready queue

    currTime=procVec[procPtr].arTime;   //sets the current time to the arrival time of the first process

    //checks the arrival of the new processes <= current time, pushes to the PQ if there are
    while(procPtr != procVecSize&&procVec[procPtr].arTime<=currTime){
        pqProcesses.push(procVec[procPtr]);
        procPtr  ++;
    }
    OutputWriter(argv[2],currTime,pqProcesses); //prints the current situation of the ready queue after adding new processes

    while(!pqProcesses.empty()) { //operates until the ready queue is emptied

        //the pointed instruction of the process at the top of the priority queue is executed,
        // currTime is set accordingly
          Process p2 = pqProcesses.top();
          pqProcesses.pop();
          currTime += p2.instrVec[p2.instrPtr];

          p2.instrPtr++;    //pointer points the next instruction of the process

          if ((p2.instrPtr) != p2.instrVec.size()) { //if current process is yet to complete
              pqProcesses.push(p2);     //if current process is completed, we do not push it in pq again

              bool newCheck=0;      //boolean denotes the arrival of a new processchecks whether a new process is arrived <= current time

              //checks the arrival of the new processes <= current time, pushes to the PQ if there are
              while (procPtr!= procVecSize&&procVec[procPtr].arTime <= currTime) {
                  pqProcesses.push(procVec[procPtr]);
                  procPtr++;
                  newCheck=1;
              }

              if(newCheck)      //prints the newly adopted ready queue's state
                  OutputWriter(argv[2],currTime,pqProcesses);
          } else {          //if current process is completed

              procVec[p2.processID].compTime=currTime;  //updates the completed process' completion time

              //checks the arrival of the new processes <= current time, pushes to the PQ if there are
              while (procPtr != procVecSize&&procVec[procPtr].arTime <= currTime) {
                  pqProcesses.push(procVec[procPtr]);
                  procPtr++;
              }
              OutputWriter(argv[2],currTime,pqProcesses); //prints the ready queue, since it's changed

          }

          //if the PQ is empty and there s processes not executed yet, sets the current time
          // to the next process' first instruction's completion time
          if ((procPtr != procVecSize)&&pqProcesses.empty()) {
              currTime = procVec[procPtr].arTime;
              //checks the arrival of the new processes <= current time, pushes to the PQ if there are
              while ((procPtr != procVecSize)&&(procVec[procPtr].arTime <= currTime)) {
                  pqProcesses.push(procVec[procPtr]);
                  procPtr++;
              }
              OutputWriter(argv[2],currTime,pqProcesses); //prints the ready queue, since it's changed

          }
    }

    /*
     * writing to the output file
     */

    ofstream out(argv[2],ios::out | ios::app);   //creates the writer object

    out<<"\n";
    for(int i=0; i<procVec.size(); i++){

        /*turnaround time= completion time- arrival time of the process
         * waiting time= turnaround time- execution time of the process
         */

        procVec[i].turnAround=procVec[i].compTime-procVec[i].arTime;
        procVec[i].waitingTime=procVec[i].turnAround-burstTimes[i];

        out<<"Turnaround time for "<< procVec[i].procName <<" = "<< procVec[i].turnAround<<" ms\n";
        out<<"Waiting time for "<< procVec[i].procName <<" = "<< procVec[i].waitingTime<<"\n";

    }
    out.close();

    return 0;
}