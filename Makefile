# CMPSC 473, Project 4

# Variables
CC=g++
LINK=g++ -lstdc++
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
	$(LINK) $(LINKFLAGS) -o $@ $(OBJS); \
		rm $(OBJS)

clean:
	rm $(OBJS) $(TARGETS)
