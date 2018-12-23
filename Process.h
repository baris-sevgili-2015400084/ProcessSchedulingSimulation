//
// Created by asus on 24.11.2018.
//

#ifndef PROJECT1_PROCESS_H
#define PROJECT1_PROCESS_H

#include <iostream>
#include <algorithm>
#include <string.h>
#include <queue>

using namespace std;

class Process {
public:

    Process(int priority, int arTime, const string &procName, const string &codeFile, int processID);

    int priority;
    int arTime;     //arrival time
    int compTime;   //completion time
    int processID;
    int turnAround;
    int waitingTime;
    int instrPtr;   //points to the next instruction to be executed
    string procName;//process name
    string codeFile;//assigned code file's name
    vector <int> instrVec;  //vector for keeping the instructions, according to the code file

};

#endif //PROJECT1_PROCESS_H
