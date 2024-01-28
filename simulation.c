#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "simulation.h"


// I simulated discrete events with a while loop, I find the next time, and then I execute the next event.
// The commented out //printf's demonstrate the process.

void mergeRemainingInstructions(int* instructionTimes, int currentInstruction, int numberOfInstructions){
    int remainingTotal = 0;
    for (int i = 0; i < numberOfInstructions - currentInstruction; i++){
        remainingTotal += instructionTimes[i + currentInstruction];
    }
    for (int i = 0; i < numberOfInstructions-1; i++){
        instructionTimes[i] = 0;
    }
    instructionTimes[0] = remainingTotal;
}

int getNextEventTimeStep(int globalTime, Queue* newQueue, struct CPU* cpu, int nextContextSwitch){
    int next = INT_MAX;
    //printf("Global time (getting the next time): %d\n", globalTime);
    //printf("Last execution time: %d\n", cpu->lastExecutionTime);
    int alreadyExecuted = (globalTime - cpu->lastExecutionTime);
    //printf("Already executed: %d\n", alreadyExecuted);
    //printf("Next context switch: %d\n", nextContextSwitch);
    if (isEmpty(newQueue) != 1){
        next = peek(newQueue)->arrivalTime - globalTime;
        if ((nextContextSwitch<=globalTime) && (cpu->occupied == 1) && (cpu->process->instructionTimes[cpu->process->currentInstruction] - alreadyExecuted < next)){
            //printf("cpu was closer in time\n");
            next = cpu->process->instructionTimes[cpu->process->currentInstruction] - alreadyExecuted;
        }
    }
    else if ((nextContextSwitch<=globalTime) && (cpu->occupied == 1)) {

        next = cpu->process->instructionTimes[cpu->process->currentInstruction] - alreadyExecuted;
    }
    if ((nextContextSwitch - globalTime < next) && (nextContextSwitch - globalTime > 0)){
        //printf("Context switch was even closer in time\n");
        next = nextContextSwitch - globalTime;
    }
    if ((next <= 0) && (globalTime != 0)){
        //printf("The next even should have been executed in the previous iteration.\n");
        exit(1);
    }
    return next;
}


int quantum(type type){
    switch(type){
        case PLATINUM:
            return 120;
        case GOLD:
            return 120;
        case SILVER:
            return 80;
    }
}

void executeInstruction(int globalTime, struct CPU* cpu, Queue* finishedQueue){
    cpu->process->timeQuantum -= cpu->process->instructionTimes[cpu->process->currentInstruction];
    cpu->process->currentInstruction++;
    cpu->lastExecutionTime = globalTime;
    //printf("Last execution time: %d\n", cpu->lastExecutionTime);
    //printf("Current instruction: %d\n", cpu->process->currentInstruction);
    //printf("Number of instructions: %d\n", cpu->process->numberOfInstructions);
    //printf("%d %s %d\n", globalTime, cpu->process->name, cpu->process->type);
    if (cpu->process->currentInstruction == cpu->process->numberOfInstructions){
        cpu->process->finishTime = globalTime;
        add(finishedQueue, cpu->process, compareArrival);
        cpu->occupied = 0;
    }
}

void preempt(int globalTime, struct CPU* cpu, Queue* readyQueue){
    cpu->process->executionCount++;
    cpu->process->lastReadyTime = globalTime;

    if (cpu->process->executionCount == 3 && cpu->process->type == SILVER){
        cpu->process->type = GOLD;
        cpu->process->executionCount = 0;
    }
    else if (cpu->process->executionCount == 5 && cpu->process->type == GOLD){
        cpu->process->type = PLATINUM;
        mergeRemainingInstructions(cpu->process->instructionTimes, cpu->process->currentInstruction, cpu->process->numberOfInstructions);
        cpu->process->numberOfInstructions = 1;
        cpu->process->currentInstruction = 0;
        cpu->process->executionCount = 0;
    }
    cpu->process->timeQuantum = quantum(cpu->process->type);
    //printf("Added %s to the ready queue preempt().\n", cpu->process->name);
    add(readyQueue, cpu->process, comparePriority);
}

void switchContext(int globalTime, struct CPU* cpu, Process* newProcess){
    cpu->occupied = 1;
    cpu->lastExecutionTime = globalTime+10;
    memcpy(cpu->process, newProcess, sizeof(Process));
    //printf("Name of the process in the cpu: %s\n", cpu->process->name);
    //Print the instruction times of the process in the cpu
    //printf("Instruction times of the process in the cpu:\n");
    //printf("%d %s %d\n", globalTime+10, cpu->process->name, cpu->process->type);
    for (int i = 0; i < cpu->process->numberOfInstructions; i++){
        //printf("%d ", cpu->process->instructionTimes[i]);
    }
    //printf("\n");
}


void simulate(Queue* newQueue, Queue* readyQueue, int numberOfProcesses, float* averageWaitTime, float* averageTurnaroundTime) {
    // What are the events that can happen?
    // 1. A process arrives to the system.
    // 2. An execution of an instruction is completed.
    // 3. An atomic process (named platinum) is completed. (Will be symmetricized with the previous one.)
    // 4. Context switch happens. (Will manually add 10 to the global time.)

    struct CPU* cpu = (struct CPU*) calloc(1, sizeof(struct CPU));
    Process* cpuProcess = (Process*) calloc(1, sizeof(Process));
    cpu->occupied = 0;
    cpu->lastExecutionTime = 0;
    cpu->process = cpuProcess;
    int globalTime = 0;
    int nextContextSwitch = 0;
    Process *tempProcess = (Process *) calloc(1, sizeof(Process));
    Queue *finishedQueue = create(numberOfProcesses);

    while (isEmpty(newQueue) != 1 || isEmpty(readyQueue) != 1 || (cpu->occupied == 1)) {
        //printf("Global time: %d\n", globalTime);
        //printf("There are %d processes in the new queue.\n", howManyProcesses(newQueue));
        //printf("The names of the processes in the new queue are:\n");
        for (int i = 0; (i < newQueue->size) && (newQueue->processes[i].finishTime != 0); i++) {
            //printf("%s ", newQueue->processes[i].name);
        }
        //printf("\n");

        //printf("There are %d processes in the ready queue.\n", howManyProcesses(readyQueue));
        //printf("The names of the processes in the ready queue are:\n");
        for (int i = 0; (i < readyQueue->size) && (readyQueue->processes[i].finishTime != 0); i++) {
            //printf("%s ", readyQueue->processes[i].name);
        }
        //printf("\n");

        //printf("The cpu is occupied: %d\n", cpu->occupied);
        if (cpu->occupied == 1) {
            //printf("The name of the process in the cpu is: %s\n", cpu->process->name);
            //printf("The current instruction of the process in the cpu is: %d\n", cpu->process->currentInstruction);
            //printf("The length of the current instruction of the process in the cpu is: %d\n", cpu->process->instructionTimes[cpu->process->currentInstruction]);
            //printf("The time quantum of the process left in the cpu is: %d\n", cpu->process->timeQuantum);
        }
        //printf("There are %d processes in the finished queue.\n", howManyProcesses(finishedQueue));


        int next = getNextEventTimeStep(globalTime, newQueue, cpu, nextContextSwitch);
        globalTime += next;
        //printf("The next event in %d.\n", globalTime);
        while ((isEmpty(newQueue) != 1) && (globalTime == peek(newQueue)->arrivalTime)) {
            //printf("Added %s to the ready queue1.\n", peek(newQueue)->name);
            add(readyQueue, removeTop(newQueue, tempProcess), comparePriority);
        }
        //printf("already executed: %d\n", globalTime - cpu->lastExecutionTime);
        if ((nextContextSwitch - globalTime < next) && (cpu->occupied == 1) && (cpu->process->instructionTimes[cpu->process->currentInstruction] == globalTime - cpu->lastExecutionTime)) {
            //printf("Executed an instruction of %s with length %d.\n", cpu->process->name, cpu->process->instructionTimes[cpu->process->currentInstruction]);
            executeInstruction(globalTime, cpu, finishedQueue);
            if (cpu->occupied == 1) {
                if (cpu->process->timeQuantum > 0) {
                    //printf("Added %s to the ready queue2.\n", cpu->process->name);
                    int putIndex = add(readyQueue, cpu->process, comparePriority);
                    //printf("Highest priority process in the ready queue: %s\n", peek(readyQueue)->name);
                    if (strcmp(peek(readyQueue)->name, cpu->process->name) != 0) {
                        //printf("SURVIVED THE IF\n");
                        removeIndex(readyQueue, putIndex);
                        preempt(globalTime, cpu, readyQueue);
                        //printf("Preempted %s\n", cpu->process->name);
                        if (strcmp(peek(readyQueue)->name, cpu->process->name) != 0) {
                            switchContext(globalTime, cpu, removeTop(readyQueue, tempProcess));
                            //printf("Will switch context to %s\n", cpu->process->name);
                            nextContextSwitch = globalTime + 10;
                        }
                        else {
                            removeTop(readyQueue, tempProcess);
                            //printf("After the preemption, we continue as the highest priority.\n");
                        }
                    }
                    else {
                        removeTop(readyQueue, tempProcess);
                        //printf("After the execution, we continue as the highest priority.\n");
                    }
                } else {
                    preempt(globalTime, cpu, readyQueue);
                    //printf("Preempted %s due to finishing of the time quantum.\n", cpu->process->name);
                    if (strcmp(peek(readyQueue)->name, cpu->process->name) != 0) {
                        //printf("Will switch context to %s\n", peek(readyQueue)->name);
                        switchContext(globalTime, cpu, removeTop(readyQueue, tempProcess));
                        nextContextSwitch = globalTime + 10;
                    } else {
                        removeTop(readyQueue, tempProcess);
                        //printf("After the preemption, we continue as the highest priority.\n");
                    }
                }
            }
        }
        if (cpu->occupied == 0) {
            if (isEmpty(readyQueue) != 1) {
                //printf("CPU has now work to do! Will switch context to %s t+=10 later.\n", peek(readyQueue)->name);
                switchContext(globalTime, cpu, removeTop(readyQueue, tempProcess));
                nextContextSwitch = globalTime + 10;
            }
        }
        if (nextContextSwitch == globalTime) {
            //printf("Context switch happened. The cpu is now occupied by %s.\n", cpu->process->name);
        }
    }
    // Calculate the average wait time and turnaround time.
    int totalWaitTime = 0;
    int totalTurnaroundTime = 0;
    int turnaroundtime = 0;
    for (int i = 0; i < numberOfProcesses; i++) {
        turnaroundtime = finishedQueue->processes[i].finishTime - finishedQueue->processes[i].arrivalTime;
        totalTurnaroundTime += turnaroundtime;
        //printf("Finish time - arrival time of %s: %d\n", finishedQueue->processes[i].name, turnaroundtime);
        totalWaitTime += turnaroundtime - finishedQueue->processes[i].totalExecution;
        //printf("Wait time of %s: %d\n", finishedQueue->processes[i].name, turnaroundtime - finishedQueue->processes[i].totalExecution);
        //printf("Index in the finishedQueue: %d\n", i);
        //printf("Number of instructions of the process in the finishedQueue: %d\n", finishedQueue->processes[i].numberOfInstructions);
        //printf("Current instruction of the process in the finishedQueue: %d\n", finishedQueue->processes[i].currentInstruction);
        //printf("Execution count of the process in the finishedQueue: %d\n", finishedQueue->processes[i].executionCount);
        //free(finishedQueue->processes[i].instructionTimes);//printf("We survived the free's.\n");
    }
    free(finishedQueue->processes);
    free(finishedQueue);
    free(tempProcess);
    free(cpu->process);
    free(cpu);
    *averageWaitTime = (float) (totalWaitTime) / numberOfProcesses;
    *averageTurnaroundTime = (float) (totalTurnaroundTime) / numberOfProcesses;
}
