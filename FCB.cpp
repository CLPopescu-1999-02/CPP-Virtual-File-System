#include "FCB.h"
#include "Constants.h"
#include <ctime>
#include <string.h>

/*
 * Constructor
 */
FCB::FCB(File* dataBlocks[],  int size,
        int permissions, char* owner, char* group) : Block()
{
    // Might have to debug dataBlocks
    memcpy(fcb.data, dataBlocks, (size / BLOCK_SIZE));

    fcb.fileSize = size;
    fcb.permissions = permissions;
    strcpy(fcb.owner, owner);
    strcpy(fcb.group, group);
}

/* Returns a pointer to the first file Block */
File* FCB::getFile() {
    return (File*) fcb.data;
}

/* Returns the file size in bytes */
int FCB::getFileSize() {
    return fcb.fileSize;
}

void FCB::setCreateTime() {
    fcb.usageTimes.create = time(NULL);
}

void FCB::setAccessTime() {
    fcb.usageTimes.access = time(NULL);
}

void FCB::setWriteTime() {
    fcb.usageTimes.write = time(NULL);
}
