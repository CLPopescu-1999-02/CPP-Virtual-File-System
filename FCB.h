/*
 * File Control Block
 *
 * Contains:
 *   - permissions
 *   - usage times - create, access, write
 *   - owner, group
 *   - access control information
 *   - size
 *   - list of data blocks
 */

#pragma once

#include "File.h"

class FCB : public Block {

public:

    /* Constructor */
    FCB(File* dataBlocks[],  int size, int permissions, char* owner, char* group);

    /* Returns a pointer to the first file Block */
    File* getFile();

    /* Returns the file size in bytes */
    int getFileSize();

    /* Set the access time */
    void setPermissions(const int permissions);

    void setCreateTime();

    /* Set the access time */
    void setAccessTime();

    /* Set the access time */
    void setWriteTime();
};
