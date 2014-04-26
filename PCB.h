#pragma once

#include <list>

#include "Block.h"
#include "Constants.h"

using namespace std;

/**
 * Object for the Process Control Block
 */
class PCB {

public:

  /* Default constructor */
  PCB();

private:

  /* Size of a block; */
  static const int blockSize = BLOCK_SIZE;

  /* Number of free blocks available */
  int freeBlockCount;

  /* List of free blocks */
  list<Block*> freeBlocks;

};
