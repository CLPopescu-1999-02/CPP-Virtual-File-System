#include <string.h>

#include <stdio.h>
#include "FBT.h"
#include "Block.h"

#include "Constants.h"

FBT::FBT(Block* freeBlocks[], int blockCount) : Block() {
    const int blocksToWrite = ((BLOCK_SIZE / sizeof(Block*)) - 1);
    memcpy(fbt.freeBlocks, freeBlocks, (blocksToWrite * sizeof(Block*)));

    // Link this to a new FBT that is passed the free blocks that haven't
    // already been stored in this current FBT.
    if ((blockCount - blocksToWrite) <= 0) {
        fbt.linkedFBT = NULL;
    } else {
        fbt.linkedFBT = new FBT(&freeBlocks[blocksToWrite], (blockCount - blocksToWrite));
    }
}

/* Get the block from the block table at index */
Block* FBT::getBlock(int index) {
    FBT* currentFBT = this;

    const int blockPtrsInBlock = ((BLOCK_SIZE / sizeof(Block*)) - 1);

    // Caculate which FBT block in the list the Block* resides
    int fbtBlockToCheck = index / blockPtrsInBlock;
    for (int i = 0; i < fbtBlockToCheck; i++) {
        // Get the next FBT linked to this one
        currentFBT = currentFBT->getLinkedFBT();
    }

    // Return the Block* in the current FBT
    return currentFBT->fbt.freeBlocks[index % blockPtrsInBlock];
};

FBT* FBT::getLinkedFBT() {
    return (FBT*) fbt.linkedFBT;
}

