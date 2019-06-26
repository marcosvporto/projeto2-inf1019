#include"swap.h"

struct swapArea {

    int maxPages;
    Page * first;
    Page * end;
};

struct swapPage{
    Page * p;
    Page * next;
    Page * prev;
};

SwapArea * s = NULL;

SwapArea loadSwapArea(FILE * f, int pageSizeBytes, int offSetBits, int pageNumBits){


    Page * p = NULL;
    int pageNumber;
    unsigned int addr;
    int auxOffSet;
    int offSet;
    char rw;

    while(fscanf(f, "%x %c ", &addr, &rw) != EOF){

        pageNumber = addr >> offSetBits;
        auxOffSet = addr << pageNumBits;
        offSet = auxOffSet >> pageNumBits;
        if(p = getPage(pageNumber) != NULL){
            
            insertAddress(p, addr, offSet, rw);

        }
        else{
            p = createPage(pageSizeBytes, addr >> offSetBits);
            insertAddress(p, addr, offSet, rw);
        }
        
        
    }


}
char swapOut (Page * p){

}
Page * swapIn(int pageNumber){

}

Page * getPage(pageNumber){
    /* Implementar busca numa lista duplamente encadeada */
    return NULL;
}





