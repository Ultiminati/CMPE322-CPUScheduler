#ifndef INC_322PROJECT2_STRUCTURES_H
#define INC_322PROJECT2_STRUCTURES_H

// I define the structures here so that I can use them in other files.
// I declared the passive form of the instruction list as a program, and the active form as a process.
// I also defined the queue and CPU structures here.
// I wish this was an object-oriented language, it would fit so well.

enum type{
    PLATINUM,
    GOLD,
    SILVER
};


typedef enum type type;

struct Program{
    char* name;
    int numberOfInstructions;
    int* instructionTimes;
};
typedef struct Program Program;

struct Process{
    int arrivalTime;
    int lastReadyTime;
    int finishTime;

    char* name;
    int priority;
    type type;

    int numberOfInstructions;
    int* instructionTimes;
    int currentInstruction;
    int totalExecution;

    int executionCount;
    int timeQuantum;

};
typedef struct Process Process;

struct Queue{
    int size;
    Process* processes;
};
typedef struct Queue Queue;

struct CPU{
    int occupied;
    int lastExecutionTime;
    Process* process;
};

#endif //INC_322PROJECT2_STRUCTURES_H
