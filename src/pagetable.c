#include "pagetable.h"

struct pageTableEntry
{
    unsigned char referenced;
    unsigned char modified;
    unsigned char present;
    unsigned char dirty;
    int chargeTime;
    int futureUsage;
};

struct pageTable
{
    int size;
    PageTableEntry * tableEntries;
};

static PageTable * tp = NULL;

void loadPageTable( int offSetBits ){

    int i;
    int totalPTEs;

    totalPTEs = 1 << (32 - offSetBits);
    PageTableEntry * pte;
    pte = (PageTableEntry *) malloc(totalPTEs*sizeof(PageTableEntry));
    tp = (PageTable*) malloc(sizeof(PageTable));
    tp->tableEntries = pte;
    tp->size = totalPTEs;
    for(i = 0; i < totalPTEs; i++){
        (tp->tableEntries+i)->chargeTime = 0;
        (tp->tableEntries+i)->dirty = 0;
        (tp->tableEntries+i)->modified = 0;
        (tp->tableEntries+i)->present = 0;
        (tp->tableEntries+i)->referenced = 0;
        (tp->tableEntries+i)->futureUsage = 0;
    }
}

int getSize(){
    return tp->size;
}
int swapIn(unsigned int addr, char rw, int offsetBits, int time){

    int pageNumber = addr >> offsetBits;
    
    (tp->tableEntries+pageNumber)->referenced = 1;
    
    if(rw == 'W'){        
        (tp->tableEntries+pageNumber)->modified= 1;
    }
    (tp->tableEntries+pageNumber)->present = 1;

    (tp->tableEntries+pageNumber)->chargeTime = time;
    return pageNumber;
}

int getPresent(unsigned int addr, int offsetBits){

    int pageNumber = addr >> offsetBits;
    return (tp->tableEntries+pageNumber)->present;

}
int swapOut_LRU(int debug){
    int i;
    int lruTime;
    int lruIndex;
    int checkedFrames = 0;
    for(i = 0; i< tp->size ; i++){

        if((tp->tableEntries+i)->present == 1){
            if(checkedFrames == 0){
                lruTime = (tp->tableEntries+i)->chargeTime;
                lruIndex = i;
                checkedFrames++;
            }
            if((tp->tableEntries+i)->chargeTime == 0){


                (tp->tableEntries+i)->present = 0;

                if(debug ==1){
                    printf("pagina %d removida com tempo de carga 0\n",i);
                    
                }
                
                if((tp->tableEntries+lruIndex)->modified == 1){

                    (tp->tableEntries+lruIndex)->modified = 0;
                    return 1; // dirty page
                }
                else{
                    return 0; //clean page
                    }

            }
            
            else{
                if((tp->tableEntries+i)->chargeTime < lruTime){

                    
                    lruTime = (tp->tableEntries+i)->chargeTime;
                    lruIndex = i;

                }
            }
        }
        checkedFrames++;
    }
    (tp->tableEntries+lruIndex)->present = 0;

    if(debug ==1){
        printf("pagina %d removida com tempo de carga %d\n",lruIndex,lruTime);
    }

    if((tp->tableEntries+lruIndex)->modified == 1){

        (tp->tableEntries+lruIndex)->modified = 0;
        return 1; // dirty page
    }
    else{
        return 0; //clean page
    }

}
int swapOut_NOVO(int debug, int time, char * fileName, int offsetBits){
    int i;
    FILE * log;
    int pageNumber;
    unsigned int addr;
    char rw;
    int lruTime;
    int lruIndex;
    int minorUsage;
    //int minorIndex = -1;
    int checkedPages=0;
    int checkedAdressess = 0;
    log = fopen(fileName,"r");
    if(fseek(log,(time)*11,SEEK_SET) == 0){
        while(fscanf(log, "%x %c ", &addr, &rw)!=EOF){
            checkedAdressess++;
            pageNumber = addr >> offsetBits;
            if((tp->tableEntries+pageNumber)->present == 1){
                if(rw == 'W'){
                    (tp->tableEntries+pageNumber)->futureUsage+=2;
                }
                else{
                    (tp->tableEntries+pageNumber)->futureUsage+=1;
                }
            }
            if(checkedAdressess > 68000){
                break;
            }

        }
        for(i=0;i<tp->size;i++){
            if( (tp->tableEntries+i)->present == 1 ){
                if(checkedPages == 0){
                    checkedPages++;
                    lruTime = (tp->tableEntries+i)->chargeTime + (tp->tableEntries+i)->futureUsage;
                    lruIndex = i;
                    minorUsage = (tp->tableEntries+i)->futureUsage;
                    //minorIndex = i;
                }
                else{
                    if((tp->tableEntries+i)->futureUsage == 0 ){

                        (tp->tableEntries+i)->present = 0;

                        if(debug ==1){
                            printf("pagina %d removida com tempo de carga 0\n",i);
                    
                        }
                
                        if((tp->tableEntries+lruIndex)->modified == 1){

                        (tp->tableEntries+lruIndex)->modified = 0;
                        return 1; // dirty page
                        }
                        else{
                            return 0; //clean page
                        }

                    }
                    if (  (tp->tableEntries+i)->chargeTime + (tp->tableEntries+i)->futureUsage < lruTime ){
                        lruTime = (tp->tableEntries+i)->chargeTime + (tp->tableEntries+i)->futureUsage;
                        lruIndex = i;
                        minorUsage = (tp->tableEntries+i)->futureUsage;
                        //minorIndex = i;
                    }
                }
            }
        }
        
    }
    fclose(log);
    // while(fscanf(log, "%x %c ", &addr, &rw)!= EOF){
    //     printf("%x %c\n",addr,rw);
    //     exit(0);
    // }
    if(debug == 1){
        printf("Pagina %d removida com uso futuro %d\n",lruIndex,minorUsage);
    }
    (tp->tableEntries+lruIndex)->present = 0;
    if((tp->tableEntries+lruIndex)->modified == 1){
        (tp->tableEntries+lruIndex)->modified = 0;
        return 1; //dirty page

    }
    return 0; //clean page
}
int swapOut_NRU(int debug){

    int i;
    for(i = 0; i < tp->size ; i++){
        if ( (tp->tableEntries+i)->present == 1 && 
                (tp->tableEntries+i)->referenced == 0 &&
                (tp->tableEntries+i)->modified == 0){

                    if(debug ==1){
                        printf("pagina %d removida R=0 M=0\n",i);
                        
                    }
                    (tp->tableEntries+i)->present = 0;
                    
                    return  0; //clean page 
        }
    }
    for(i = 0; i < tp->size ; i++){
        if ( (tp->tableEntries+i)->present == 1 && 
                (tp->tableEntries+i)->referenced == 0 &&
                (tp->tableEntries+i)->modified == 1){



            (tp->tableEntries+i)->present = 0;

            (tp->tableEntries+i)->modified = 0;

            if(debug ==1){
                        printf("pagina %d removida R=0 M=1\n",i);
                        //sleep(1);
            }

            return  1;//dirty page
        }
    }
    for(i = 0; i < tp->size ; i++){
        if ( (tp->tableEntries+i)->present == 1 && 
                (tp->tableEntries+i)->referenced == 1 &&
                (tp->tableEntries+i)->modified == 0){

            (tp->tableEntries+i)->present = 0;
            
            if(debug ==1){
                        printf("pagina %d removida R=1 M=0\n",i);
                        
                        
            }
            return  0; // clean page
        }
    }
    for(i = 0; i < tp->size ; i++){
        if ( (tp->tableEntries+i)->present == 1 &&
                (tp->tableEntries+i)->referenced == 1 &&
                (tp->tableEntries+i)->modified == 1){

            (tp->tableEntries+i)->present = 0;
            (tp->tableEntries+i)->modified = 0;

            if(debug ==1){
                        printf("pagina %d removida R=1 M=1\n",i);
                        //sleep(1);
            }
            return 1; // dirty page
        }
    }
    return -1;
}

void setNotRecentlyUsed (){
    int i;
    for(i = 0; i< tp->size ; i++){
        (tp->tableEntries+i)->chargeTime = 0;
        (tp->tableEntries+i)->referenced = 0;
        (tp->tableEntries+i)->futureUsage = 0;
        
    }
    
}



