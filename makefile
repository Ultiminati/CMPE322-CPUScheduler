scheduler: main.o simulation.o queueAndSort.o
	gcc main.o simulation.o queueAndSort.o -o scheduler

main.o: main.c simulation.h
	gcc -c main.c

simulation.o: simulation.c simulation.h
	gcc -c simulation.c

queueAndSort.o: queueAndSort.c queueAndSort.h
	gcc -c queueAndSort.c

