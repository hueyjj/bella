CC		= gcc
DEBUG	= -DDEBUGPRINT -Wall
FLAGS 	= --std=c99 -mwindows -O2 

all: bella
	
bella: 			bellamain.o bella.o
	$(CC)	$(FLAGS) -o bella.exe bellamain.o bella.o

bellamain.o: 	bellamain.c bella.h
	$(CC) 	$(FLAGS) -c bellamain.c

bella.o: 		bella.c bella.h
	$(CC)	$(FLAGS) -c bella.c

debug: FLAGS += $(DEBUG)
debug: bella

clean: 
	rm -f bella.exe bellamain.o bella.o
