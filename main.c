#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"



//void initializeInstructions(char* fileName, char** instructionNames, int* instructionTimes);
Program* initializePrograms();
int initializeProcessesAndQueues(char* fileName, Program* programs, Queue** newQueue);
void printResults(float averageWaitTime, float averageTurnaroundTime);

//Maybe populate the queues in the reverse order to speed up the insertion process.

//DON'T INITIALIZE THE FINISHTIME FIELD WITH A 0, I CHECK EMPTYNESS WITH THAT.
int main() {
    //Decided to hardcode the instructions and the programs.

    Program* programs = initializePrograms();
    //printf("Programs initialized.\n");
    // create the programs

    Queue** newQueueArg = (Queue**) calloc(1, sizeof(Queue*));
    Queue* readyQueue;
    int numberOfProcesses = initializeProcessesAndQueues("definition.txt", programs,newQueueArg);
    Queue* newQueue = *newQueueArg;
    free(newQueueArg);
    readyQueue = create(numberOfProcesses);
    // Read definition.txt, get the total number of processes, create processes with the help of the programs,
    // populate the newQueue sorted based on the arrival times of the processes and create the readyQueue.
    //printf("Processes and queues initialized.\n");


    //printf("Number of processes: %d\n", numberOfProcesses);

    // Start the simulation.
    float* averageWaitTime = (float*) calloc(1, sizeof(float));
    float* averageTurnaroundTime = (float*) calloc(1, sizeof(float));
    simulate(newQueue, readyQueue, numberOfProcesses, averageWaitTime, averageTurnaroundTime);
    //printf("Simulation finished.\n");



    // Print the results.
    printResults(*averageWaitTime, *averageTurnaroundTime);
    //printf("Results printed.\n");

    free(programs);
    //printf("Programs freed.\n");

    free(newQueue);
    //printf("New queue freed.\n");

    free(readyQueue);
    //printf("Ready queue freed.\n");

    free(averageWaitTime);
    //printf("Average wait time freed.\n");
    free(averageTurnaroundTime);
    //printf("Average turnaround time freed.\n");
    return 0;
}


// I decided to hardcode the instructions and the programs.
Program* initializePrograms(){
    int instructions[21]={90, 80, 70, 60, 50, 40, 30, 20, 30, 40, 50, 60, 70, 80, 90, 80, 70, 60, 50, 40, 10};
    Program* programs = (Program*) calloc(10, sizeof(Program));

    int p1_t[12]={1, 19, 15, 18, 3, 2, 20, 15, 18, 3, 2, 21};
    int* p1_times = (int*) calloc(12, sizeof(int));
    for (int i = 0; i < 12; i++){
        p1_times[i] = instructions[p1_t[i]-1];
    }
    Program p1 = {"P1", 12, p1_times};
    memcpy(programs, &p1, sizeof(Program));


    int p2_t[9]={18, 2, 5, 6, 5, 6, 5, 6, 21};
    int* p2_times = (int*) calloc(9, sizeof(int));
    for (int i = 0; i < 9; i++){
        p2_times[i] = instructions[p2_t[i]-1];
    }
    Program p2 = {"P2", 9, p2_times};
    memcpy(programs+1, &p2, sizeof(Program));


    int p3_t[10]={8, 7, 12, 11, 13, 16, 19, 8, 7, 21};
    int* p3_times = (int*) calloc(10, sizeof(int));
    for (int i = 0; i < 10; i++){
        p3_times[i] = instructions[p3_t[i]-1];
    }
    Program p3 = {"P3", 10, p3_times};
    memcpy(programs+2, &p3, sizeof(Program));

    int p4_t[6]={9, 2, 19, 9, 2, 21};
    int* p4_times = (int*) calloc(6, sizeof(int));
    for (int i = 0; i < 6; i++){
        p4_times[i] = instructions[p4_t[i]-1];
    }
    Program p4 = {"P4", 6, p4_times};
    memcpy(programs+3, &p4, sizeof(Program));


    int p5_t[11]={9, 2, 19, 9, 2, 2, 19, 9, 2, 19, 21};
    int* p5_times = (int*) calloc(11, sizeof(int));
    for (int i = 0; i < 11; i++){
        p5_times[i] = instructions[p5_t[i]-1];
    }
    Program p5 = {"P5", 11, p5_times};
    memcpy(programs+4, &p5, sizeof(Program));

    int p6_t[15]={10, 9, 20, 11, 4, 5, 7, 10, 9, 20, 11, 4, 5, 7, 21};
    int* p6_times = (int*) calloc(15, sizeof(int));
    for (int i = 0; i < 15; i++){
        p6_times[i] = instructions[p6_t[i]-1];
    }
    Program p6 = {"P6", 15, p6_times};
    memcpy(programs+5, &p6, sizeof(Program));


    int p7_t[6]={8, 1, 10, 11, 2, 21};
    int* p7_times = (int*) calloc(6, sizeof(int));
    for (int i = 0; i < 6; i++){
        p7_times[i] = instructions[p7_t[i]-1];
    }
    Program p7 = {"P7", 6, p7_times};
    memcpy(programs+6, &p7, sizeof(Program));


    int p8_t[5]={14, 4, 3, 1, 21};
    int* p8_times = (int*) calloc(5, sizeof(int));
    for (int i = 0; i < 5; i++){
        p8_times[i] = instructions[p8_t[i]-1];
    }
    Program p8 = {"P8", 5, p8_times};
    memcpy(programs+7, &p8, sizeof(Program));


    int p9_t[6]={19, 12, 9, 1, 7, 21};
    int* p9_times = (int*) calloc(6, sizeof(int));
    for (int i = 0; i < 6; i++){
        p9_times[i] = instructions[p9_t[i]-1];
    }
    Program p9 = {"P9", 6, p9_times};
    memcpy(programs+8, &p9, sizeof(Program));


    int p10_t[10]={20, 3, 19, 5, 2, 11,8,13,14,21};
    int* p10_times = (int*) calloc(10, sizeof(int));
    for (int i = 0; i < 10; i++){
        p10_times[i] = instructions[p10_t[i]-1];
    }
    Program p10 = {"P10", 10, p10_times};
    memcpy(programs+9, &p10, sizeof(Program));

    return programs;
}

//
int mapType(char* type){
    if (strncmp(type, "PLATINUM", 8) == 0){
        return PLATINUM;
    }
    else if (strncmp(type, "GOLD", 4) == 0){
        return GOLD;
    }
    else if (strncmp(type, "SILVER", 6) == 0){
        return SILVER;
    }
    else{
        //printf("Error reading type.\n");
        exit(1);
    }
}

// I map the program names to their indices in the programs array.
int mapProgram(char* programName, Program* programs){
    for (int i = 0; i < 10; i++){
        if (strcmp(programName, programs[i].name) == 0){
            return i;
        }
    }
    //printf("Error reading program.\n");
    exit(1);
}

// I initialize the processes and the queues.
int initializeProcessesAndQueues(char* fileName, Program* programs, Queue** newQueueArg){
    FILE* file = fopen(fileName, "r");
    if (file == NULL){
        //printf("Error opening file.\n");
        exit(1);
    }
    Process tempProcesses[1024];
    int numberOfProcesses = 0;
    char* name = (char*) calloc(9, sizeof(char));
    int priority[1];
    int arrivalTime[1];
    char type[8];
    while (fscanf(file, "%s %d %d %s", name, priority, arrivalTime, type) > 0){
        numberOfProcesses++;
        Process process;
        char* actualName = (char*) calloc(strlen(name)+1, sizeof(char));
        strncpy(actualName, name, strlen(name));
        //printf("%s %d %d %s\n", actualName, *priority, *arrivalTime, type);
        process.name = actualName;
        process.priority = *priority;
        process.arrivalTime = *arrivalTime;
        process.lastReadyTime = *arrivalTime;
        process.finishTime = -1;
        process.type = mapType(type);
        process.numberOfInstructions = programs[mapProgram(actualName, programs)].numberOfInstructions;
        memcpy(&process.instructionTimes, &programs[mapProgram(actualName, programs)].instructionTimes, sizeof(int*));
        process.currentInstruction = 0;
        process.executionCount = 0;
        process.timeQuantum = quantum(process.type);
        memcpy(&tempProcesses[numberOfProcesses-1], &process, sizeof(Process));
    }
    fclose(file);
    free(name);

    *newQueueArg = create(numberOfProcesses);
    for (int i = 0; i < numberOfProcesses; i++){
        if (tempProcesses[i].type == PLATINUM){
            //printf("Instruction times of %s are:\n", tempProcesses[i].name);
            for (int j = 0; j < tempProcesses[i].numberOfInstructions; j++){
                //printf("%d ", tempProcesses[i].instructionTimes[j]);
            }
            mergeRemainingInstructions(tempProcesses[i].instructionTimes, 0, tempProcesses[i].numberOfInstructions);
            tempProcesses[i].numberOfInstructions = 1;
            //printf("Instruction times of %s are:\n", tempProcesses[i].name);
            for (int j = 0; j < tempProcesses[i].numberOfInstructions; j++){
                //printf("%d ", tempProcesses[i].instructionTimes[j]);
            }
            //printf("Sum of them: %d\n", tempProcesses[i].instructionTimes[0]);
        }
        for (int j = 0; j < tempProcesses[i].numberOfInstructions; j++){
            tempProcesses[i].totalExecution += tempProcesses[i].instructionTimes[j];
        }
        add(*newQueueArg, tempProcesses+i, compareArrival);
    }
    return numberOfProcesses;
}

// I print the results.
void printResults(float averageWaitTime, float averageTurnaroundTime){
    averageWaitTime - (int)(averageWaitTime) == 0 ? printf("%d\n",(int)averageWaitTime) : printf("%.1f\n", averageWaitTime);
    averageTurnaroundTime - (int)(averageTurnaroundTime) == 0 ? printf("%d\n",(int)averageTurnaroundTime) : printf("%.1f\n", averageTurnaroundTime);
}
