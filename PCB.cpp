#include "PCB.h"

#include "Constants.h"
#include "FBT.h"

PCB::PCB(FileSystem fs) : Block() {

    pcb.blockSize = BLOCK_SIZE;

    // This block is the first block of the disk, so one less free block
    pcb.freeBlockCount = NUM_OF_BLOCKS - 1;

    // Create the free block table
    pcb.freeBlockTable = new FBT(&fs.getBlocks()[1], pcb.freeBlockCount);

    // Index into the FBT
    pcb.nextFreeBlock = 0;
}

Block* PCB::getFreeBlock() {
    // If no free blocks, return -1
    if (pcb.freeBlockCount > NUM_OF_BLOCKS) {
        return NULL;
    }

    // Save the index of the nextFreeBlock to return
    int freeBlockIndex = pcb.nextFreeBlock;

    // Increment the nextFreeBlock index
    pcb.nextFreeBlock++;

    // Decrease the number of free blocks
    pcb.freeBlockCount--;

    return ((FBT*) pcb.freeBlockTable)->getBlock(freeBlockIndex);
}
