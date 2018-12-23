//
// Created by asus on 24.11.2018.
//

#include <fstream>
#include "Process.h"

Process::Process(int priority, int arTime, const string &procName, const string &codeFile, int processID)   {
   this->priority=priority;
   this->arTime=arTime;     //arrival time
   this->procName=procName; //process name
   this->codeFile=codeFile; //assigned code file's name
   this->compTime=0;        //completion time of the process
   this->instrPtr=0;        //points to the next instruction to be executed
   this->processID=processID; //ID of the process (which is the line number in the file)

   int turnAround=0;
   int waitingTime=0;
   ifstream codeInput(this->codeFile+".txt", ios::in); //creates the writer object, opens the output file
   string trash;        //trash string to arrange the pulling necessary information from file
   int exTime;          //execution time of the instruction
    //reads the input (assigned code file), pushes the instructions to the instructions vector
   while (codeInput.good()){
       codeInput >> trash >> exTime;
       this->instrVec.push_back(exTime);
       if(trash=="exit")        //exits if exit instruction comes
           break;
   }
   codeInput.close();

 }


