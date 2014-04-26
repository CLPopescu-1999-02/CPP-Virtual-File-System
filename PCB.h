#pragma once

#include <list>
#include <vector>

#include "Block.h"
#include "Constants.h"
#include "FileSystem.h"

using namespace std;

/**
 * Object for the Process Control Block
 */
class PCB : public Block {

public:

  /* Default constructor */
  PCB();

  /* Block* getFreeBlock(); */
  int getFreeBlock();

private:

  /* Size of a block; */
  static const int blockSize = BLOCK_SIZE;

  /* Number of free blocks available */
  int freeBlockCount;

  /* Index the next free block */
  int nextFreeBlock;
};
