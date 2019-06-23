#include "pagetable.h"

struct pageTableEntry
{
    unsigned char referenced;
    unsigned char modified;
    unsigned char protection;
    unsigned char present;
    unsigned int frameNumber;
};

struct pageTable
{
    int size;
    PageTableEntry * tableEntries;

};

PageTable * tp = NULL;

PageTable * loadPageTable( int offSetBits ){

    int totalPTEs;
    totalPTEs = 1 << (32 - offSetBits);
    
    PageTableEntry * pte;
    pte = (PageTableEntry *) malloc(totalPTEs*sizeof(PageTableEntry));
    tp = (PageTable*) malloc(sizeof(PageTable));
    tp->tableEntries = pte;
    tp->size = totalPTEs;
    return tp;
}

int getSize(){
    return tp->size;
}


