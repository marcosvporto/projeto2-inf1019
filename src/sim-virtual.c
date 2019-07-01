
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include"pagetable.h"

#define MegaBytes 1048576
#define KiloBytes 1024


void alarmHandler(int signal);

int deltaT = 1;
int debug = 0;
int time = 0;

void loadResources(int memSize, int pageSize, int *totalFrames, int *offSetBits){


    int memBytes, pageBytes, tFrames, mOffset;
    memBytes = memSize * MegaBytes; // Total of physical memory in bytes
    pageBytes = pageSize * KiloBytes; // Size of a page in bytes
    
    switch (pageSize)
    {
    case 8:{
        mOffset = 13;
    }break;
    
    case 16:{
        mOffset = 14;

    }break;

    case 32:{
        mOffset = 15;

    }break;

    default:{
        mOffset = 13;
    }
        break;
    }

    tFrames = memBytes / pageBytes; 

    *totalFrames = tFrames;

    *offSetBits = mOffset;

}
int main (int argc, char const* argv []){

    int dirtyPages = 0;
    
    int pageHits = 0;
    int pageFaults = 0;
    int occupiedFrames = 0;
    unsigned int addr;
    char rw;
    int offSetBits;
    int totalFrames;
    int pageSize;
    int memSize;
    FILE * log;
    char * logVerify = (char *) malloc(4*sizeof(char *));
    char * logFileName = (char *) malloc (strlen(argv[2])*sizeof(char *));
    signal(SIGALRM,alarmHandler);

    if(argc < 5 || argc > 6){
        printf("Quantidade de argumentos invalido\n");
        return 0;
    }
    if(strcmp(argv[1],"LRU") != 0 &&
       strcmp(argv[1],"NRU") != 0 &&
       strcmp(argv[1],"NOVO")!= 0 ){

        printf("algoritmo de substituição de páginas inválido\n");
        return 0;
    }
    if(argc==6 && strcmp(argv[5],"-D") == 0){
        printf("debug\n");
        debug =1;
        deltaT  = 30;
    }
    
    strcpy(logVerify,argv[2]);
    
    strcpy(logVerify, logVerify+(strlen(logVerify)-4));
    
    if(strcmp(logVerify,".log") != 0 ){
        printf("arquivo de log invalido\n");
    }
    strcpy(logFileName,argv[2]);


    pageSize = atoi(argv[3]);
    memSize = atoi(argv[4]);
    if(pageSize != 8  &&
       pageSize != 16 &&
       pageSize != 32 ){

           printf("tamanho de pagina invalido \n");
           return 0;
    
       }
    if(memSize < 1 ||
       memSize > 16){
           printf("tamanho da memoria invalido \n");
           return 0;
       }
    

    log = fopen(logFileName,"r");
    if(log == NULL){
        printf("Erro ao abrir o arquivo log\n");
        return 0;
    }
    
    printf("Executando o simulador...\n");

    loadResources(memSize,pageSize, &totalFrames, &offSetBits);

    loadPageTable(offSetBits);

    alarm(deltaT);

    while(fscanf(log, "%x %c ", &addr, &rw) != EOF){
        //sleep(0.7);
        time++;
        //printf("%d,",time);
        // if(time % totalFrames == 0){
        //     setNotRecentlyUsed();
        // }
        if( getPresent(addr,offSetBits) ){
            // if(debug == 1){
            //     printf("page Hit\n");
            //     sleep(1);
            // }
            pageHits++;
            swapIn(addr,rw,offSetBits,time);
            //printf("Page Hit - Página %d ja esta na memoria\n",swapIn(addr,rw,offSetBits));
        }
        else{
            if(occupiedFrames < totalFrames){
                swapIn(addr,rw,offSetBits,time);
                if(debug){
                    printf("Página %d agora esta na memoria\n",addr>>offSetBits);
                }
                //printf("Página %d agora esta na memoria -",swapIn(addr,rw,offSetBits));
                occupiedFrames++;
                if(debug == 1){
                    printf("%d quadros preenchidos\n",occupiedFrames);
                    //sleep(1);
                }
                
            }
            else{

                if(debug == 1){
                    printf("Page Fault !!! \n");
                }
                pageFaults++;
                if(strcmp(argv[1],"NRU") == 0){
                    dirtyPages+= swapOut_NRU(debug);
                    //printf("Pagina %d removida\n", swapOut_NRU());
                }
                else if(strcmp(argv[1],"LRU") == 0){
                    dirtyPages+= swapOut_LRU(debug);
                    //printf("Pagina %d removida\n", swapOut_LRU());
                }
                else if(strcmp(argv[1],"NOVO")==0){
                    //*(sizeof(unsigned int)+strlen(" ")+sizeof(char)+strlen("\0"))
                    dirtyPages+=swapOut_NOVO(debug, time, logFileName, offSetBits);
                }
                swapIn(addr,rw,offSetBits,time);
                if(debug){
                    printf("Página %d agora esta na memoria RW = %c\n",addr>>offSetBits, rw);
                }
                // printf("Página %d agora esta na memoria apos substituicao\n",swapIn(addr,rw,offSetBits));
                //exit(0);

            }
            
        }
        

    }


    //printf("tamanho da TP %d \n",getSize());
    printf("Arquivo de entrada: %s\n",argv[2]);
    printf("Tamanho da memoria fisica: %s MB\n", argv[4]);
    printf("Tamanho das páginas: %s KB\n",argv[3]);
    printf("Algoritmo de substituição: %s\n", argv[1]);
    printf("Numero de Faltas de Paginas: %d\n",pageFaults);
    printf("Escritas de Páginas escritas: %d\n",dirtyPages);
    

    


    return 0;
}


void alarmHandler(int signal){
    //time = 0;
    setNotRecentlyUsed();
    if(debug){
        printf("--------------------------clock--------------------------------------\n");
        sleep(1);
        alarm(300);
    }
    else{
        alarm(deltaT);
    }
}