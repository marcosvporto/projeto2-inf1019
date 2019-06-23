
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include"pagetable.h"

#define MegaBytes 1048576
#define KiloBytes 1024



void loadResources(int memSize, int pageSize, int totalAccess, int *totalFrames, int *offSetBits){


    int memBytes, pageBytes, accessPerPage, tFrames, mOffset;
    //accessBytes = totalAccess * 4; // Each access address has 32 bit i.e. 4 bytes
    memBytes = memSize * MegaBytes; // Total of physical memory in bytes
    pageBytes = pageSize * KiloBytes; // Size of a page in bytes

    accessPerPage = pageBytes / 4; // How many access adresses fit in a page


    printf("Acessos por página %d\n", accessPerPage);
    printf("Tamanho de uma pagina em bytes %d\n", pageBytes);
    printf("Tamanho da memoria em bytes %d\n", memBytes);

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

    unsigned int addr;
    char rw;
    int i;
    int totalAccesses = 0;
    int offSetBits;
    int pageNumBits;
    int totalFrames;
    int pageSize;
    int memSize;
    FILE * log;
    PageTable * tp;
    char * logVerify = (char *) malloc(4*sizeof(char *));
    char * logFileName = (char *) malloc (strlen(argv[2])*sizeof(char *));

    for(i = 0; i < argc ; i++){
        printf(" argumento %d = %s\n",i, argv[i]);
    }
    if(argc != 5){
        printf("Quantidade de argumentos invalido\n");
        return 0;
    }
    if(strcmp(argv[1],"LRU") != 0 &&
       strcmp(argv[1],"NRU") != 0 &&
       strcmp(argv[1],"NOVO")!= 0 ){

        printf("algoritmo de substituição de páginas inválido\n");
        return 0;
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
    while(fscanf(log, "%x %c ", &addr, &rw) != EOF){

        totalAccesses++;

    }

    loadResources(memSize,pageSize,totalAccesses, &totalFrames, &offSetBits);
    pageNumBits = 32 - offSetBits;
    printf("Quantidade de quadros a serem criados %d\n", totalFrames);
    printf("Numero de bits de offset %d\n", offSetBits);
    printf("Quantidade de entradas da TP %d \n", 1<<pageNumBits);

    
    tp = loadPageTable( offSetBits );

    printf("tamanho da TP %d \n",getSize());


    


    return 0;
}