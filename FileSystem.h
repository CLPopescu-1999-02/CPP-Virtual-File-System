#pragma once

#include "Directory.h"
#include "PCB.h"
#include "Block.h"
#include "FCB.h"


class FileSystem {

public:
    FileSystem();
    Block** getBlocks();
    void addBlock(Block* block);
    Directory* getRoot();

private:
    PCB* getPCB();
    Block* blocks[NUM_OF_BLOCKS];
};
