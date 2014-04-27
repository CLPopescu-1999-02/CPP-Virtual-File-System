#include "PCB.h"

#include "Constants.h"
#include "FBT.h"

PCB::PCB(Block* blocks[]) : Block() {

    printf("1");
    pcb.blockSize = BLOCK_SIZE;

    // This block is the first block of the disk, so one less free block
    printf("2");
    pcb.freeBlockCount = NUM_OF_BLOCKS - 1;

    // Create the free block table
    printf("3");
    pcb.freeBlockTable = new FBT(&blocks[1], pcb.freeBlockCount);

    // Index into the FBT
    printf("4");
    pcb.nextFreeBlock = 0;
}

Block* PCB::getFreeBlock() {
    // If no free blocks, return -1
    printf("1");
    if (pcb.freeBlockCount > NUM_OF_BLOCKS) {
        return NULL;
    }

    printf("1");
    // Save the index of the nextFreeBlock to return
    int freeBlockIndex = pcb.nextFreeBlock;

    printf("1");
    // Increment the nextFreeBlock index
    pcb.nextFreeBlock++;

    printf("3");
    // Decrease the number of free blocks
    pcb.freeBlockCount--;

    return ((FBT*) pcb.freeBlockTable)->getBlock(freeBlockIndex);
}
