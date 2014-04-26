#include "PCB.h"

#include "Constants.h"

PCB::PCB() : Block() {
    // This block is the first block of the disk, so one less free block
    freeBlockCount = NUM_OF_BLOCKS - 1;

    // Next free block is the second block in the file system since the PCB is
    // the first.
    /* nextFreeBlock = blocks[1]; */
    nextFreeBlock = 1;
}

int PCB::getFreeBlock() {
    // If no free blocks, return -1
    if (freeBlockCount > NUM_OF_BLOCKS) {
        return -1;
    }

    // Save the index of the nextFreeBlock to return
    int freeBlockIndex = nextFreeBlock;

    // Increment the nextFreeBlock index
    nextFreeBlock++;

    // Decrease the number of free blocks
    freeBlockCount--;

    return freeBlockIndex;
}
