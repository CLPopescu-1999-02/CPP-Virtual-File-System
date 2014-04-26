#pragma once

#include "Directory.h"
#include "PCB.h"
#include "Block.h"
#include "FCB.h"
#include "PCB.h"

int debug = 0;          // extra output; 1 = on, 0 = off


class FileSystem {

public:
    FileSystem();
    Block** getBlocks();
    void addBlock(Block* block);

private:
    PCB* getPCB();
    /* PCB* pcb; */
    Block* blocks[NUM_OF_BLOCKS];
};
