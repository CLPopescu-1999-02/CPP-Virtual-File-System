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

};
