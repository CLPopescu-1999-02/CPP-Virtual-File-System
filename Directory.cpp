#include "Directory.h"
#include "Block.h"
#include "string.h"
#include <time.h>


Directory::Directory(char* name) : Block() {
    strcpy(directory.header.dirName, name);
    setCreateTime();
}

int Directory::addEntry(char* name, Block* data) {
    if (getEntryCount() < getMaxEntries()) {
        strcpy(directory.entrys[getEntryCount()].entryName, name);
        directory.entrys[getEntryCount()].data = data;

        directory.header.entryCount++;

        setAccessTime();
        setWriteTime();
        return  getEntryCount();
    }

    return -1;
}

int Directory::getEntryCount() {
    return directory.header.entryCount;
}

int Directory::getMaxEntries() {
    return directory.header.maxEntries;
}

void Directory::setCreateTime() {
    directory.header.usageTimes.create = time(NULL);
}

void Directory::setAccessTime() {
    directory.header.usageTimes.access = time(NULL);
}

void Directory::setWriteTime() {
    directory.header.usageTimes.write = time(NULL);
}
