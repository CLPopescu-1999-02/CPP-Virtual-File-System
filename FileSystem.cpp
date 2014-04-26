
#include "FileSystem.h"

FileSystem::FileSystem() {
    blocks[0] = new PCB();
}

Block** FileSystem::getBlocks() {
    return blocks;
}

PCB* FileSystem::getPCB() {
    return (PCB*) &blocks[0];
}

void FileSystem::addBlock(Block* block) {
    blocks[getPCB()->getFreeBlock()] = block;
}
