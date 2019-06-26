#include"page.h"

struct page{

    int number;
    unsigned int * addr;
    char * control;

};

Page * createPage(int sizeBytes, int number){
    Page * p = (Page *) malloc(sizeof(Page));
    p->addr = (unsigned int *)malloc(sizeBytes*sizeof(unsigned int));
    return p; 
}

char insertAddress(Page * p, unsigned int addr,int offSet, char control){

    if(p != NULL){
        *(p->addr+offSet) = addr;
        *(p->control+offSet) = control;
        return 1;
    }
    return 0;
}