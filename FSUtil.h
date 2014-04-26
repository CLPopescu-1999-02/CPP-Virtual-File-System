#pragma once

#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/* #include "FileSystem.h" */
/* #include "Directory.h" */
#include "Constants.h"

/*
 * This class initializes the file system and provides functions to operate on
 * it.
 */
class FSUtil {

public:
    FSUtil();

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
    /* FileSystem fileSys; */

    /* The current directory to operate on */
    /* Directory curDir; */

    /* The previous directory */
    // Might just keep a reference in the directory object
    /* Directory prevDir; */
};
