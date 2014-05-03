## Project 4 - File System ##

A file system simulation written in C++

---

#### FSUtil: ####

**File System Util:** All the commands to act on the file system (e.g. mkdir,
cd, rm, etc.)

#### Block: ####

The base unit of the File System

#### File System: ####

The whole file system. Has references to the PCB and all the blocks.

#### PCB: ####

**Partition Control Block:** holds information about the file systems (i.e.
block size, pointer to free block table)

#### FBT: ####

**Free Block Table:** A table with information about the free blocks

#### Directory: ####

Holds some meta data and all names of all the files and directory entries
contained in this directory.

#### FCB: ####

**File Control Block:** holds information about a file (i.e. permissions,
pointers to the blocks that contains the file, usage times)
