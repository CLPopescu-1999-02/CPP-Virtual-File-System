#include <stdio.h>
#include <string.h>

#include "Constants.h"
#include "Block.h"


Block::Block(){
}

int Block::getType() {
    return 0;
}

Block::Block(char* bytesToCopy, int size){
  memcpy(blockBytes, bytesToCopy, size);
}



