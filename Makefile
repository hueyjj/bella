CC		= gcc
FLAGS 	= --std=c99

all: bella
	
bella: 			bellamain.o bella.o
	$(CC)	$(FLAGS) -o bella.exe bellamain.o bella.o

bellamain.o: 	bellamain.c bella.h
	$(CC) 	$(FLAGS) -c bellamain.c

bella.o: 		bella.c bella.h
	$(CC)	$(FLAGS) -c bella.c

clean: 
	rm -f bella.exe bellamain.o bella.o
