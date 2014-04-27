#pragma once

#include <list>
#include <vector>

#include "Block.h"
#include "Constants.h"

using namespace std;

/**
 * Object for the Process Control Block
 */
class PCB : public Block {

public:

  /* Default constructor */
  PCB(Block* blocks[]);

  /* Block* getFreeBlock(); */
  Block* getFreeBlock();
};
