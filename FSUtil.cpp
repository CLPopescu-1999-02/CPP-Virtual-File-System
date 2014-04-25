#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "FSUtil.h"

#define LINESIZE 128
#define NAME_SIZE 50

int FSUtil::do_root(char *name, char *size) {

  if (debug) printf("%s\n", __func__);

  fileSystem = new FileSystem();
  currentDirectory = fileSystem.getRoot();

  memcpy ( dirPtr->dirName, name, strlen(name)+1 );

  dirPtr->firstSub = NULL;
  dirPtr->nextSub = NULL;
  dirPtr->prevDir = NULL;

  return 0;
}

int FSUtil::do_print(char *name, char *size) {
  if (debug) printf("%s\n", __func__);
  return -1;
}


/*******************************************************************************
* Change directory function.
* (cd ..) to move up a directory
* (cd "nameOfDir") to move to the subdirectory called nameOfDir
********************************************************************************/
int FSUtil::do_chdir(char *name, char *size) {
    if (debug) printf("%s\n", __func__);

	//Move up a directory
	if(strcmp(name, "..") == 0)
	{
        printf("do chdir : chdir root\n");
		if(dirPtr->prevDir == NULL)
			printf("This is the root directory\n");
		else
			dirPtr = dirPtr->prevDir;
		return 0;
	}
	//Move down a directory
	else
	{
        printf("do chdir : name %s\n", name);
		if(dirPtr->firstSub == NULL)
		{
			printf("Subdirectory %s does not exist.\n", name);
			return -1;
		}
		else dirPtr = dirPtr->firstSub;
        printf("do chdir : name %s, first sub name %s, next sub name %s \n", name,dirPtr->firstSub->dirName,dirPtr->nextSub->dirName);
		if(strcmp(name, dirPtr->dirName) == 0)
		{
			return 0;
		}

		while(dirPtr->nextSub != NULL)
		{
			if(strcmp(name, dirPtr->nextSub->dirName) == 0)
			{
				dirPtr = dirPtr->nextSub;
				return 0;
			}
			else
			{
				dirPtr = dirPtr->nextSub;

			}
		}
		//Change back to original directory if the subdirectory does not exist.
		dirPtr = dirPtr->prevDir;
		printf("Subdirectory %s does not exist.\n", name);
	}

	return -1; //failure
}

/*******************************************************************************
* Make directory function.
* (mkdir name) creates a subdirectory in the current directory called name
* Length of name must not exceed maximum length, NAME_SIZE
* Directory must not already exist in current directory
********************************************************************************/
//?????????Still need to allocate space in new block??????????????????-------------------------------------
int FSUtil::do_mkdir(char *name, char *size) {

	if (debug) printf("%s\n", __func__);

        printf("do mkdir : name %s dirptrname %s\n", name, dirPtr->dirName);
	if(sizeof(name) > NAME_SIZE)
	{
		printf("The name of the directory is too long. Try again.\n");
		return -1;
	}

        Direct *newDir = new Direct;
	newDir->nextSub = NULL;
	newDir->firstSub = NULL;
	newDir->prevDir = dirPtr;
  	memcpy ( dirPtr->dirName, name, strlen(name)+1 );

	if(dirPtr->firstSub == NULL) //If the current directory has no subdirectories
	{
		dirPtr->firstSub = newDir;

                delete newDir;
		return 0;
	}
        printf("do mkdir : dirptrname %s dirptrfirstsubname %s name %s\n", dirPtr->dirName, dirPtr->firstSub->dirName, name);
	dirPtr = dirPtr->firstSub;

        if(strcmp(name, dirPtr->dirName) == 0)
	{
		printf("Subdirectory %s already exists\n", name);
		dirPtr = dirPtr->prevDir;
		return -1;
	}

	if(dirPtr->nextSub == NULL) //If the current directory only has 1 subdirectory
	{
		dirPtr->nextSub = newDir;
		return 0;
	}

	while(dirPtr->nextSub != NULL) //change to last sub directory
	{
		dirPtr = dirPtr->nextSub;
		if(strcmp(name, dirPtr->dirName) == 0) //Check if the subdirectory already exists
		{
			printf("Subdirectory %s already exists.\n", name);
			dirPtr = dirPtr->prevDir;
			return -1;
		}

	}

	dirPtr->nextSub = newDir;

	//change back to directory where new dir was created
	dirPtr = dirPtr->prevDir;

	return 0;
}

/*******************************************************************************
* Remove directory function.
* (rmdir name) deletes the subdirectory name and everything in it recursively
* Directory must exist in current directory
********************************************************************************/
//when exiting this function, dirPtr should be the directory from which you started
int FSUtil::do_rmdir(char *name, char *size) {
{
	if (debug) printf("%s\n", __func__);

	//Reject request to delete directory .. or .
	if(strcmp(name, "..") == 0 || strcmp(name, ".") == 0)
	{
		printf("Move up a level to remove directory.\n");
		return -1;
	}

	//Reject request to delete root directory
	if(strcmp(name, dirPtr->dirName) == 0 && dirPtr->prevDir == NULL)
	{
		printf("Cannot delete root directory.\n");
		return -1;
	}

	if(dirPtr->firstSub != NULL)
	{
		if(strcmp(name, dirPtr->firstSub->dirName) == 0)
			dirPtr = dirPtr->firstSub;
		else
		{
			dirPtr = dirPtr->firstSub;
			while(dirPtr->nextSub != NULL)
			{
				dirPtr = dirPtr->nextSub;
				if(strcmp(name, dirPtr->dirName) == 0)
				{
					break;
				}
			}

			if(strcmp(name, dirPtr->dirName) != 0)
			{
				printf("Subdirectory %s does not exist in current directory.\n", name);
				dirPtr = dirPtr->prevDir;
				return -1;
			}
		}
	}
	else
	{
		printf("Subdirectory %s does not exist in current directory.\n", name);
		return -1;
	}

	//at this point, we are in the directory to be deleted

	//base case, no subdirectories in directory to be deleted
	if(dirPtr->firstSub == NULL)
	{
		dirPtr = dirPtr->prevDir;
		if(strcmp(name, dirPtr->firstSub->dirName) == 0)
		{
			//do temp directory stuff
			//release block
			dirPtr->firstSub = dirPtr->firstSub->nextSub;
			printf("Deleting %s in 1\n", name);
			return 0;
		}
		else
		{
			dirPtr = dirPtr->firstSub;
			while(dirPtr->nextSub != NULL)
			{
				if(strcmp(name, dirPtr->nextSub->dirName) == 0)
				{
					//do temp directory stuff
					//release block
					dirPtr->nextSub = dirPtr->nextSub->nextSub;
					dirPtr = dirPtr->prevDir;
					printf("Deleting %s in 2\n", name);
					return 0;
				}
				dirPtr = dirPtr->nextSub;
			}
		}
	}

	//second case, if there are at least two subdirectories
	while(dirPtr->firstSub->nextSub != NULL)
	{
		do_rmdir(dirPtr->firstSub->nextSub->dirName, 0);
	}

	//last case, only one subdirectory
	do_rmdir(dirPtr->firstSub->dirName, 0);

	dirPtr = dirPtr->prevDir;
	do_rmdir(name, 0);

	return 0;
}

/*******************************************************************************
* Rename directory function
* (mvdir old_name new_name) will rename the directory called old_name to new_name
*  in the current directory
* The directory old_name must be in the current directory
********************************************************************************/
//Does this check only current directory subdirectories?????????????????????????????????????
int FSUtil::do_mvdir(char *name, char *size) {
	if (debug) printf("%s\n", __func__);

	if(dirPtr->firstSub == NULL)
	{
		printf("Directory %s does not existt.\n", old_name);
		return -1;
	}
	dirPtr = dirPtr->firstSub;
	if(strcmp(old_name, dirPtr->dirName) == 0)
	{
		memcpy ( dirPtr->dirName, new_name, strlen(new_name)+1 );
		dirPtr = dirPtr->prevDir;
		return 0;
	}

	while(dirPtr->nextSub != NULL)
	{
		dirPtr = dirPtr->nextSub;

		if(strcmp(old_name, dirPtr->dirName) == 0)
		{
			memcpy ( dirPtr->dirName, new_name, strlen(new_name)+1 ); //Rename directory
			//Change back to original directory
			dirPtr = dirPtr->prevDir;
			return 0;
		}
	}

	if(dirPtr->nextSub == NULL)
	{
		printf("Directory %s does not exist.\n", old_name);
	}

	dirPtr = dirPtr->prevDir;

	return -1;//failure
}

int FSUtil::do_mkfil(char *name, char *size) {
  return 0;
}

int FSUtil::do_rmfil(char *name, char *size) {
  return 0;
}

int FSUtil::do_mvfil(char *name, char *size) {
  return 0;
}

int FSUtil::do_szfil(char *name, char *size) {
  return 0;
}

int FSUtil::do_exit(char *name, char *size) {
  return 0;
}

/*--------------------------------------------------------------------------------*/
