CC		= gcc
FLAGS 	= --std=c99

all: bella
	
bella: 			bellamain.o bella.o
	$(CC)	-o bella.exe bellamain.o bella.o

bellamain.o: 	bellamain.c bella.h
	$(CC) 	-c bellamain.c

bella.o: 		bella.c bella.h
	$(CC)	-c bella.c

clean: 
	rm -f bella.exe bellamain.c bella.c
