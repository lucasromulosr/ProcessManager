all: commander manager reporter

commander: commander.c
	gcc -c commander.c
	gcc -o commander commander.o

manager: manager.c manager_TAD.c process.c
	gcc -c manager.c manager_TAD.c process.c
	gcc -o manager manager_TAD.o manager.o process.o

reporter: reporter.c
	gcc -c reporter.c
	gcc -o reporter reporter.o
	
clean:
	rm -f manager commander reporter
	rm -f *.o
