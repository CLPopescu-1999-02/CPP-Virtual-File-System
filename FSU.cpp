#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "FSU.h"
#include "Constants.h"

int debug = 0;
struct File{
    char fileName[NAME_SIZE];
    int size;
    //pointer to the file control block
};

struct Directory{
  char dirName[NAME_SIZE];
  Directory* nextSub;
  Directory* firstSub;
  Directory* prevDir;
  //list of FCB
  //pointer to next block
        std::vector<File> files; //????need to set size
};

struct Directory *dirPtr = new Directory;

int FSU::do_root(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);

  strcpy(dirPtr->dirName, "Root");
  dirPtr->firstSub = NULL;
  dirPtr->nextSub = NULL;
  dirPtr->prevDir = NULL;

  return 0;
}

/*******************************************************************************
* Print function
* This function is similar to "ls -lR".
* It will print the name of the current directory and list its files, then
*   recurse over its subdirectories.
*******************************************************************************/

int FSU::do_print(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  
  printf("\nDirectory: %s\n", dirPtr->dirName);
  //print out files????????????????????????????????????????
  for(int i = 0; i < dirPtr->files.size(); i++)
      printf("File: %s   %d\n", dirPtr->files[i].fileName, dirPtr->files[i].size);

  //if there are no subdirectories
  if(dirPtr->firstSub == NULL)
      return 0;

  //if there is only one subdirectory
  if(dirPtr->firstSub->nextSub == NULL)
  {
      printf("Directory: %s\n", dirPtr->firstSub->dirName);
  }
  else
  {
      dirPtr = dirPtr->firstSub;
      printf("Directory: %s\n", dirPtr->dirName);
      while(dirPtr->nextSub != NULL)
      {
          dirPtr = dirPtr->nextSub;
          printf("Directory: %s\n", dirPtr->dirName);
      }
      //change back to previous directory
      dirPtr = dirPtr->prevDir;
  }

  //at this point, the files in the directory are printed
  //now print the subdirectories, if any
  
  //recurse over first subdirectory
  dirPtr = dirPtr->firstSub;
  do_print("dummy", 0);
  
  //recurse over the rest of the subdirectories
  while(dirPtr->nextSub != NULL)
  {
      dirPtr = dirPtr->nextSub;
      do_print("dummy", 0);
  }
  //change back to previous directory
  dirPtr = dirPtr->prevDir;

  return 0;
}

/*******************************************************************************
* Change directory function.
* (cd ..) to move up a directory
* (cd "nameOfDir") to move to the subdirectory called nameOfDir
********************************************************************************/
int FSU::do_chdir(char * name, char * size)
{
  if (debug) printf("%s\n", __func__);

  //Move up a directory
  if(strcmp(name, "..") == 0)
  {
    if(dirPtr->prevDir == NULL)
      printf("This is the root directory\n");
    else
      dirPtr = dirPtr->prevDir;
    return 0;
  }
  //Move down a directory
  else
  {
    if(dirPtr->firstSub == NULL)
    {
      printf("Subdirectory %s does not exist.\n", name);
      return -1;
    }
    dirPtr = dirPtr->firstSub;
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
int FSU::do_mkdir(char * name, char * size)
{
  if (debug) printf("%s\n", __func__);
        
  if(strlen(name) > NAME_SIZE)
  {
    printf("The name of the directory is too long. Try again.\n");
    return -1;
  }
  Directory *newDir = new Directory();
  newDir->nextSub = NULL;
  newDir->firstSub = NULL;
  newDir->prevDir = dirPtr;
  strcpy(newDir->dirName, name);

  if(dirPtr->firstSub == NULL) //If the current directory has no subdirectories
  {
    dirPtr->firstSub = newDir;
    return 0;
  }
  dirPtr = dirPtr->firstSub;
  
        if(strcmp(name, dirPtr->dirName) == 0)
  {
    printf("Subdirectory %s already exists.1\n", name);
    dirPtr = dirPtr->prevDir;
                
    return -1;
  }

  if(dirPtr->nextSub == NULL) //If the current directory only has 1 subdirectory
  {
    dirPtr->nextSub = newDir;
                dirPtr = dirPtr->prevDir;
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
int FSU::do_rmdir(char * name, char * size)
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
      //printf("Deleting %s in 1\n", name);
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
          //printf("Deleting %s in 2\n", name);
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
int FSU::do_mvdir(char *old_name, char *new_name)
{
  if (debug) printf("%s\n", __func__);

  if(dirPtr->firstSub == NULL)
  {
    printf("Directory %s does not exist.\n", old_name);
    return -1;
  }
  dirPtr = dirPtr->firstSub;
  if(strcmp(old_name, dirPtr->dirName) == 0)
  {
    strcpy(dirPtr->dirName, new_name);
    dirPtr = dirPtr->prevDir;
    return 0;
  }

  while(dirPtr->nextSub != NULL)
  {
    dirPtr = dirPtr->nextSub;
    
    if(strcmp(old_name, dirPtr->dirName) == 0)
    {
      strcpy(dirPtr->dirName, new_name); //Rename directory
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

int FSU::do_mkfil(char *name, char *size)
{
  //?????????Ask for block, do FCB stuff, do size stuff?????????????????????
  if (debug) printf("%s\n", __func__);
  
  if(strlen(name) > NAME_SIZE)
  {
      printf("The size of the name is too big.\n");
      return -1;
  }

  if(dirPtr->files.size() > NUM_FILES)
  {
      printf("Maximum number of files reached.\n");
      return -1;
  }

  struct File newFile;
  strcpy(newFile.fileName, name);
  newFile.size = atoi(size);
  dirPtr->files.push_back(newFile);
  return 0;
}

int FSU::do_rmfil(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);

  for(int i = 0; i < dirPtr->files.size(); i++)
  {
      //if file is found
      if(strcmp(dirPtr->files[i].fileName, name) == 0)
      {
          dirPtr->files.erase(dirPtr->files.begin()+i);
          return 0;
      }
  }
  return -1;
}
  
int FSU::do_mvfil(char *old_name, char *new_name)
{
  if (debug) printf("%s\n", __func__);
  
  for(int i = 0; i < dirPtr->files.size(); i++)
  {
        //if file is found
       if(strcmp(dirPtr->files[i].fileName, old_name) == 0)
       {
           strcpy(dirPtr->files[i].fileName, new_name);
           return 0;
       }
  }

  //file is not found
  printf("File %s does not exist.\n", old_name);
  return -1;
}

int FSU::do_szfil(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  
  for(int i = 0; i < dirPtr->files.size(); i++)
  {
    //if file is found
    if(strcmp(dirPtr->files[i].fileName, name) == 0)
    {
      dirPtr->files[i].size = atoi(size);
      return 0;
    }
  }

  //file is not found
  printf("File %s does not exist.\n", name);
  return -1;
}

int FSU::do_exit(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  exit(0);
  return 0;
}

