#include "FileSystem.h"
#include "PCB.h"

FileSystem::FileSystem() {
    blocks[0] = new PCB(blocks);
}

Block** FileSystem::getBlocks() {
    return blocks;
}

PCB* FileSystem::getPCB() {
    return (PCB*) &blocks[0];
}

void FileSystem::addBlock(Block* block) {
    return;
}

Directory* FileSystem::getRoot() {
    Directory* root = new Directory("Root");
    Block* freeBlock = getPCB()->getFreeBlock();
    freeBlock = root;

    return root; 
}
