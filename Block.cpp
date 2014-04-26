#include <stdio.h>
#include <string.h>

#include "Constants.h"
#include "Block.h"


Block::Block(){
  bytes = new char[BLOCK_SIZE];
}

Block::Block(char* bytesToCopy, int size){
  memcpy(bytes, bytesToCopy, size);
}



