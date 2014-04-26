#pragma once

#include "Constants.h"

struct UsageTimes {
    int create;
    int access;
    int write;
};

/*
 * A basic disk block object
 */
class Block {

public:
    Block();
    Block(char * bytesToCopy, int size);

protected:
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
                UsageTimes usageTimes;
                int entryCount;
                char dirName[48];
                Block* prevDir;
                Block* linkedDir;
                static const int maxEntries = 30;
            } header;

            struct Entry {
                char entryName[48];
                Block* data;
            } entrys[(BLOCK_SIZE - sizeof(Header) - sizeof(int)) / sizeof(Entry)];

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
