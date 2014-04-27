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

    virtual int getType();

protected:
    union {
        // Byte layout for a free block table
        struct {
            /* Block* freeBlocks [BLOCK_SIZE/sizeof(Block*) - 1]; */
            Block* freeBlocks;
            Block* linkedFBT;
        } fbt;

        // Bytes layout for a partition control block
        struct {
            int blockSize;
            int freeBlockCount;
            int nextFreeBlock;
            int blocksPerFBT;
            Block* freeBlockTable;
        } pcb;

        struct {
            char permissions;
            UsageTimes usageTimes;
            char owner[48];
            char group[48];
            int fileSize;
            Block* data[30]; // Should have MAX_DATA attribute
        } fcb;

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
    char entryName[48];
    Block* data;
};


