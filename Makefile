CC		= gcc
DEBUG	= -DDEBUGPRINT -g #-Wall
STATIC	= -static -static-libgcc -static-libstdc++
FLAGS 	= --std=c99 -mwindows -O2 

all: bella
	
bella: 			bellamain.o bella.o bella-icon.o 
	$(CC)	$(FLAGS) -o bella.exe src/bellamain.o src/bella.o src/bella-icon.o
	rm	src/bellamain.o src/bella.o src/bella-icon.o

bellamain.o: 	src/bellamain.c src/bella.h src/resource.h
	$(CC) 	$(FLAGS) -o src/bellamain.o -c src/bellamain.c

bella.o: 		src/bella.c src/bella.h
	$(CC)	$(FLAGS) -o src/bella.o -c src/bella.c

bella-icon.o:   src/bella-icon.rc src/resource.h
	windres src/bella-icon.rc src/bella-icon.o

debug: FLAGS += $(DEBUG)
debug: bella

static: FLAGS += $(STATIC)
static: bella

clean: 
	rm -f bella.exe src/bellamain.o src/bella.o src/bella-icon.o
