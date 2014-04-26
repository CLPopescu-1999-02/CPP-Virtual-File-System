#pragma once

#include "Constants.h"

/*
 * A basic disk block object
 */
class Block {

public:
    Block();
    Block(char * bytesToCopy, int size);

private:

    union {
        struct {
            Block* nextBlock;
            char safeBytes[BLOCK_SIZE - sizeof(Block*)];
        };
        char allBytes[BLOCK_SIZE];
    };
};
