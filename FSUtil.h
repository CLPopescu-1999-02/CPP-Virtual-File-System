#pragma once

#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "Constants.h"
#include "FileSystem.h"
#include "Directory.h"


/*
 * This class initializes the file system and provides functions to operate on
 * it.
 */
class FSUtil {

public:
    int do_root (char *name, char *size);
    int do_print(char *name, char *size);
    int do_chdir(char *name, char *size);
    int do_mkdir(char *name, char *size);
    int do_rmdir(char *name, char *size);
    int do_mvdir(char *name, char *size);
    int do_mkfil(char *name, char *size);
    int do_rmfil(char *name, char *size);
    int do_mvfil(char *name, char *size);
    int do_szfil(char *name, char *size);
    int do_exit (char *name, char *size);

private:
    /* The whole file system */
    FileSystem* fs;

    /* The current directory to operate on */
    Directory* curDir;

};
