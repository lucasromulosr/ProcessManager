all: commander manager

commander: commander.c
	gcc -c commander.c -Wall
	gcc -o commander commander.o

manager: manager.c manager_TAD.c process.c
	gcc -c manager.c manager_TAD.c process.c -Wall
	gcc -o manager manager_TAD.o manager.o process.o

clean:
	rm -f manager commander
	rm -f *.o
