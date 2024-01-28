#ifndef INC_322PROJECT2_SIMULATION_H
#define INC_322PROJECT2_SIMULATION_H
#include "structures.h"
#include "queueAndSort.h"

void executeInstruction(int globalTime, struct CPU* cpu, Queue* finishedQueue);
void mergeRemainingInstructions(int* instructionTimes, int currentInstruction, int numberOfInstructions);
void switchContext(int globalTime, struct CPU* cpu, Process* newProcess);
void preempt(int globalTime, struct CPU* cpu, Queue* readyQueue);
int quantum(type type);

int getNextEventTimeStep(int globalTime, Queue* newQueue, struct CPU* cpu, int nextContextSwitch);
void simulate(Queue* newQueue, Queue* readyQueue, int numberOfProcesses, float* averageWaitTime, float* averageTurnaroundTime);


#endif //INC_322PROJECT2_SIMULATION_H
