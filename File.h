#include "Block.h"

class File : public Block {

public:
    File();
    File(char* bytes, int size);
};
