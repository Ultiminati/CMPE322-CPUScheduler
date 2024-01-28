#ifndef INC_322PROJECT2_QUEUEANDSORT_H
#define INC_322PROJECT2_QUEUEANDSORT_H
#include "structures.h"


int comparePriority(Process* p1, Process* p2);
int compareArrival(Process* p1, Process* p2);

Queue* create(int size);
int add(Queue* processQueue, Process* newProcess, int (*compareTo)(Process* p1, Process* p2));
Process* peek(Queue* processQueue);
Process* removeTop(Queue* processQueue, Process* top);
Process* removeIndex(Queue* processQueue, int index);
void printQueue(Queue* processQueue);
int isEmpty(Queue* processQueue);
int howManyProcesses(Queue* processQueue);

#endif //INC_322PROJECT2_QUEUEANDSORT_H
