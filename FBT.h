#pragma once

#include "Block.h"
#include "Constants.h"

/**
 * Object for the Process Control Block
 */
class FBT : public Block {

public:

    FBT(Block* freeBlocks[], int blockCount);
    Block* getBlock(int index);

private:
    FBT* getLinkedFBT();
};
