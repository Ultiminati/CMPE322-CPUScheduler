#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queueAndSort.h"

// I implemented a priority queue with a sorted array.
// I pass the comparison functions as arguments to the add function.
// This structure acts like a normal priority queue, but not terribly efficient.

int compareArrival(Process* p1, Process* p2){
    if (p1->lastReadyTime < p2->lastReadyTime){
        return -1;
    }
    else if (p1->lastReadyTime > p2->lastReadyTime){
        return 1;
    }
    else{
        return 0;
    }
}


int comparePriority(Process* p1, Process* p2) {
    if ((p1->type == PLATINUM) && (p2->type == PLATINUM)){
        if (p1->priority < p2->priority) return 1;
        else if (p1->priority > p2->priority) return -1;
        else {
            int arrival = compareArrival(p1, p2);
            if (arrival == 0) return strcmp(p1->name, p2->name);
            else return arrival;
        }
    }
    else if ((p1->type == PLATINUM) || (p2->type == PLATINUM)) {
        if (p1->type == PLATINUM) return -1;
        else return 1;
    }
    else if (p1->priority < p2->priority) return 1;
    else if (p1->priority > p2->priority) return -1;
    else {
        int arrival = compareArrival(p1, p2);
        if (arrival == 0){
            if (strcmp(p1->name, p2->name) < 0) return -1;
            else if (strcmp(p1->name, p2->name) > 0) return 1;
            else return 0;
        }
        else return arrival;
    }
}


Queue* create(int size){
    Process* processes = (Process*) calloc(size, sizeof(Process));
    Queue* processQueue = (Queue*) calloc(1, sizeof(Queue));
    processQueue->processes = processes;
    processQueue->size = size;
    return processQueue;
}

int isEmpty(Queue* processQueue){
    if ((processQueue->size == 0) || (processQueue->processes[0].finishTime == 0)){
        return 1;
    }
    else{
        return 0;
    }
}

int howManyProcesses(Queue* processQueue){
    if (isEmpty(processQueue) == 1){
        return 0;
    }
    int i = 0;
    while ((i<processQueue->size-1) && (processQueue->processes[i+1].finishTime != 0)){
        i++;
    }
    return i+1;
}

int add(Queue* processQueue, Process* newProcess, int (*compareTo)(Process* p1, Process* p2)){
    // The last element that is not NULL is the highest priority element. It is sorted in reverse.
    // very error-prone code
    Process* temp = processQueue->processes;
    int i = 0;
    while (temp[i].finishTime != 0){
        i++;
        if (i == processQueue->size){
            //printf("Queue is full.\n");
            //printQueue(processQueue);
            exit(1);
        }
    }
    int j = i-1;
    while (j >= 0 && compareTo(processQueue->processes+j, newProcess) == -1) {
        j--;
    }
    j++;
    for (int k = i; k > j; k--){
        memcpy(&processQueue->processes[k], &processQueue->processes[k-1], sizeof(Process));
    }
    memcpy(&processQueue->processes[j], newProcess, sizeof(Process));
    return j;
}

Process* peek(Queue* processQueue){
    Process* pr = processQueue->processes;
    int i = 0;
    while ((i<processQueue->size-1) && ((pr+i+1)->finishTime != 0)){
        i++;
    }
    pr += i;
    return pr;
}

Process* removeTop(Queue* processQueue, Process* top){
    // Also check this code again if errors occur
    Process* pr = processQueue->processes;
    int i = 0;
    while ((i<processQueue->size) && (pr[i].finishTime != 0)){
        i++;
    }
    if (i == 0){
        //printf("Queue is empty.\n");
        exit(1);
    }
    i--;

    memcpy(top, &pr[i], sizeof(Process));
    memset(&pr[i], 0, sizeof(Process));
    return top;
}

Process* removeIndex(Queue* processQueue, int index){
    Process* pr = processQueue->processes;
    int i = 0;
    while ((i<processQueue->size) && (pr[i].finishTime != 0)){
        i++;
    }
    if (i == 0){
        //printf("Queue is empty.\n");
        exit(1);
    }
    i--;
    //printf("Before removing, the finish time of all processes is:\n");
    for (int j = 0; j < i+1; j++){
        //printf("%d ", pr[j].finishTime);
    }
    //printf("\n");
    for (int k = index; k < i; k++){
        memcpy(processQueue->processes+k, processQueue->processes+k+1, sizeof(Process));
    }
    memset(processQueue->processes+i, 0, sizeof(Process));
    //printf("After removing, the finish time of all processes is:\n");
    for (int j = 0; j < i; j++){
        //printf("%d ", pr[j].finishTime);
    }
    //printf("\n");
}

void printQueue(Queue* priorityQueue) {
    for (int i = 0; (priorityQueue->processes[i].finishTime != 0) && (i < priorityQueue->size); i++) {
        //printf("Name: %s\n", priorityQueue->processes[i].name);
        //printf("Priority: %d\n", priorityQueue->processes[i].priority);
        //printf("Arrival Time: %d\n", priorityQueue->processes[i].arrivalTime);
        //printf("Last ready time:%d\n", priorityQueue->processes[i].lastReadyTime);
        //printf("Finish time:%d\n", priorityQueue->processes[i].finishTime);
        //printf("Type:%d\n", priorityQueue->processes[i].type);
        //printf("Number of instructions:%d\n", priorityQueue->processes[i].numberOfInstructions);
        //printf("Current instruction:%d\n", priorityQueue->processes[i].currentInstruction);
        //printf("Execution count:%d\n", priorityQueue->processes[i].executionCount);
        //printf("Time quantum: %d\n", priorityQueue->processes[i].timeQuantum);
    }
    if (priorityQueue->processes[0].finishTime == 0){
        //printf("Queue is empty.\n");
    }
}