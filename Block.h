#pragma once

/*
 * A basic disk block object
 */
class Block {

public:
    Block();
    Block(char *);

private:
    char* bytes;
};
