#pragma once

#include <list>
#include <string>

#include "Constants.h"

using namespace std;

class Directory {

public:

    /* Constructor for directory */
    Directory();

    /* Create a FCB class */
    int createFCB(int permissions, char* owner, char* group, int size);

private:

    char name[NAME_SIZE];

    /* Pointer to the previous directory (AKA '..') */
    Directory* prevDir;

    /**List of the names of the files or the directories */
    list<string> fileNames;

};

