#include "FCB.h"
#include <ctime>

/*
 * Constructor
 */
FCB::FCB(int permissions, char* owner, char* group, int size) {
    this->permissions = permissions;
    this->owner = owner;
    this->group = group;
    this->size = size;

    // Set times
    this->usage_times.create = std::time(NULL);
}
