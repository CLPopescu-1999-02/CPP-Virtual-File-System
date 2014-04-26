#include "Block.h"

class Directory : public Block {

public:
    Directory(char* name);

    int addEntry(char* name, Block* data);

    int getEntryCount();
    int getMaxEntries();
    void setCreateTime();
    void setAccessTime();
    void setWriteTime();
};
