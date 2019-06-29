#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <string.h>


typedef struct pageTable PageTable;
typedef struct pageTableEntry PageTableEntry;

void loadPageTable( int offSetBits );

int swapIn(unsigned int addr, char rw, int offsetBits, int time);

int swapOut_NRU(int debug);

int swapOut_LRU(int debug);

void setNotRecentlyUsed ();

int getPresent(unsigned int addr, int offsetBits);

int getSize();




