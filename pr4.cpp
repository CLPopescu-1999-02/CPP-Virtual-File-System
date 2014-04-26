#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*--------------------------------------------------------------------------------*/

int debug = 0;	// extra output; 1 = on, 0 = off

/*--------------------------------------------------------------------------------*/

/* The input file (stdin) represents a sequence of file-system commands,
 * which all look like     cmd filename filesize
 *
 * command	action
 * -------	------
 *  root	initialize root directory
 *  print	print current working directory and all descendants
 *  chdir	change current working directory
 *                (.. refers to parent directory, as in Unix)
 *  mkdir	sub-directory create (mk = make)
 *  rmdir	              delete (rm = delete)
 *  mvdir	              rename (mv = move)
 *  mkfil	file create
 *  rmfil	     delete
 *  mvfil	     rename
 *  szfil	     resize (sz = size)
 *  exit        quit the program immediately
 */

//Global variables
const int NAME_SIZE = 50;

struct Directory{
	char dirName[NAME_SIZE];
	Directory* nextSub;
	Directory* firstSub;
	Directory* prevDir;
	//list of FCB
	//pointer to next block
};

struct Directory *dirPtr = new Directory;

/* The size argument is usually ignored.
 * The return value is 0 (success) or -1 (failure).
 */
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

struct action {
  char *cmd;					// pointer to string
  int (*action)(char *name, char *size);	// pointer to function
} table[] = {
    { "root" , do_root  },
    { "print", do_print },
    { "chdir", do_chdir },
    { "mkdir", do_mkdir },
    { "rmdir", do_rmdir },
    { "mvdir", do_mvdir },
    { "mkfil", do_mkfil },
    { "rmfil", do_rmfil },
    { "mvfil", do_mvfil },
    { "szfil", do_szfil },
    { "exit" , do_exit  },
    { NULL, NULL }	// end marker, do not remove
};

/*--------------------------------------------------------------------------------*/

void parse(char *buf, int *argc, char *argv[]);

#define LINESIZE 128

/*--------------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
  char in[LINESIZE];
  char *cmd, *fnm, *fsz;
  char dummy[] = "";

  int n;
  char *a[LINESIZE];

  while (fgets(in, LINESIZE, stdin) != NULL)
    {
      // commands are all like "cmd filename filesize\n" with whitespace between

      // parse in
      parse(in, &n, a);

      cmd = (n > 0) ? a[0] : dummy;
      fnm = (n > 1) ? a[1] : dummy;
      fsz = (n > 2) ? a[2] : dummy;
      if (debug) printf(":%s:%s:%s:\n", cmd, fnm, fsz);

      if (n == 0) continue;	// blank line

      int found = 0;
      for (struct action *ptr = table; ptr->cmd != NULL; ptr++)
        {
	  if (strcmp(ptr->cmd, cmd) == 0)
	    {
	      found = 1;
	      int ret = (ptr->action)(fnm, fsz);
	      if (ret == -1)
	        { printf("  %s %s %s: failed\n", cmd, fnm, fsz); }
	      break;
	    }
	}
      if (!found)
        { printf("command not found: %s\n", cmd); }
    }

  return 0;
}

/*--------------------------------------------------------------------------------*/

int do_root(char *name, char *size)
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

int do_print(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  
  printf("\nDirectory: %s\n", dirPtr->dirName);
  //print out files????????????????????????????????????????
  //if there are no subdirectories
  if(dirPtr->firstSub == NULL)
      return 0;

  //if there is only one subdirectory
  if(dirPtr->firstSub->nextSub == NULL)
  {
      printf("%s\n", dirPtr->firstSub->dirName);
  }
  else
  {
      dirPtr = dirPtr->firstSub;
      printf("%s\n", dirPtr->dirName);
      while(dirPtr->nextSub != NULL)
      {
          dirPtr = dirPtr->nextSub;
          printf("%s\n", dirPtr->dirName);
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
int do_chdir(char * name, char * size)
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
int do_mkdir(char * name, char * size)
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
int do_rmdir(char * name, char * size)
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
int do_mvdir(char *old_name, char *new_name)
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

int do_mkfil(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  return -1;
}

int do_rmfil(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  return -1;
}

int do_mvfil(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  return -1;
}

int do_szfil(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  return -1;
}

int do_exit(char *name, char *size)
{
  if (debug) printf("%s\n", __func__);
  exit(0);
  return 0;
}

/*--------------------------------------------------------------------------------*/

// parse a command line, where buf came from fgets()

// Note - the trailing '\n' in buf is whitespace, and we need it as a delimiter.

void parse(char *buf, int *argc, char *argv[])
{
  char *delim;          // points to first space delimiter
  int count = 0;        // number of args

  char whsp[] = " \t\n\v\f\r";          // whitespace characters

  while (1)                             // build the argv list
    {
      buf += strspn(buf, whsp);         // skip leading whitespace
      delim = strpbrk(buf, whsp);       // next whitespace char or NULL
      if (delim == NULL)                // end of line
        { break; }
      argv[count++] = buf;              // start argv[i]
      *delim = '\0';                    // terminate argv[i]
      buf = delim + 1;                  // start argv[i+1]?
    }
  argv[count] = NULL;

  *argc = count;

  return;
}

/*--------------------------------------------------------------------------------*/


