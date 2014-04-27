# CMPSC 473, Project 4

# Variables
CC=g++ -std=c++0x
LINK=gcc -std=gnu99
CFLAGS=-Wall -Wextra
LINKFLAGS=-g

# Files to build

OBJS=	main.o \
	FSUtil.o \
	FileSystem.o \
	PCB.o \
	FBT.o \
	Directory.o \
	File.o \
	FCB.o \
	Block.o

TARGETS = main

# Suffix rules
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

dummy : $(TARGETS)

main: $(OBJS)
	$(LINK) $(LINKFLAGS) -o $@ $(OBJS)

clean:
	rm $(OBJS) $(TARGETS)
