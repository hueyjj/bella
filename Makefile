CC		= gcc
DEBUG	= -DDEBUGPRINT #-Wall
STATIC	= -static -static-libgcc -static-libstdc++
FLAGS 	= --std=c99 -mwindows -O2 

all: bella
	
bella: 			bellamain.o bella.o
	$(CC)	$(FLAGS) -o bella.exe src/bellamain.o src/bella.o
	rm	src/bellamain.o src/bella.o

bellamain.o: 	src/bellamain.c src/bella.h
	$(CC) 	$(FLAGS) -o src/bellamain.o -c src/bellamain.c

bella.o: 		src/bella.c src/bella.h
	$(CC)	$(FLAGS) -o src/bella.o -c src/bella.c

debug: FLAGS += $(DEBUG)
debug: bella

static: FLAGS += $(STATIC)
static: bella

clean: 
	rm -f bella.exe src/bellamain.o src/bella.o
