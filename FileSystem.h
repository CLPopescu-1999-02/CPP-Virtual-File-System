#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Directory.h"
#include "PCB.h"
#include "Block.h"
#include "FCB.h"

#define LINESIZE        128
#define NAME_SIZE       50
#define SIZE_OF_BLOCK   4096
#define NUM_OF_BLOCKS   1000*SIZE_OF_BLOCK

int debug = 0;          // extra output; 1 = on, 0 = off

#pragma once

class FileSystem {

public:
private:
    PCB pcb;
    Block blocks[NUM_OF_BLOCKS];
    FCB pcb;
};
