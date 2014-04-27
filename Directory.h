#pragma once

#include "Block.h"

class Directory : public Block {

public:
    Directory(char* name);
    Directory(char* name, Directory* prevDir);

    virtual int getType();
    int addEntry(char* name, Block* data);

    int getEntryCount();
    int getMaxEntries();
    Entry* getEntries();
    char* getName();
    Directory* getPrevDir();
    void setCreateTime();
    void setAccessTime();
    void setWriteTime();
};
