#pragma once

#include "Constants.h"
#include "Directory.h"

/*
 * A basic disk block object
 */
class Block {

public:
    Block();
    Block(char * bytesToCopy, int size);

private:
    union {
        // Byte layout for a free block table
        struct {
            Block* nextBlock;
            Block* freeBlocks [BLOCK_SIZE/sizeof(Block*) - 1];
        } fbt;

        // Bytes layout for a partition control block
        struct {
            int blockSize;
            int freeBlockCount;
            int nextFreeBlock;
            Block* freeBlockTable;
        } pcb;

        // Bytes layout for a file
        struct {
            struct Header {
                char dirName[48];
                Directory* prevDir;
                Directory* linkedDir;
            } header;

            struct Entry {
                char entryName[48];
                Block* block;
            } entrys[(BLOCK_SIZE - sizeof(Header)) / sizeof(Entry)];

        } directory;

        // Bytes layout for a file
        struct {
          char bytes[BLOCK_SIZE];
        } file;

        // Holds the union so it is no more or less that BLOCK_SIZE bytes
        char blockBytes[BLOCK_SIZE];
    };
};

struct Entry {
    char* name;
    Block* dataBlock;
};
