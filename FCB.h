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

#include <list>

using namespace std;

class FCB {

public:

    /* Constructor */
    FCB(int permissions, char* owner, char* group, int size);

    /* Set the access time */
    void setPermissions(const int permissions) { this->permissions = permissions; }

    /* Set the access time */
    void setAccessTime(const int accessTime) { this->usage_times.access = accessTime; }

    /* Set the access time */
    void setWriteTime(const int writeTime) { this->usage_times.write = writeTime; }

private:

    /* Permission bits for the file (4 - read, 2 - write, 1 - execute) */
    char permissions;

    /* Different usage time associated with the file */
    struct Usage_times {
        int create;
        int access;
        int write;
    } usage_times;

    /* User the file belongs to */
    char* owner;

    /* Group that the file belongs to */
    char* group;

    /* Size of the file in bytes */
    int size;

    /* Linked list of pointers to the data blocks */
    list<char> dataBlocks;
};
