#ifndef directory_h
#define directory_h

class Directory {

public:
	Directory();								 /* Constructor for directory */
	int CreateFCB(int permissions, char* owner, char* group, int size);		/* Create a FCB class */
	int CreatePCB();	 

private:


Directory * dptr;
Directory * ptrnxt;
    
};


#endif directory_h
