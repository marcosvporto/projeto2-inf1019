#include<stdlib.h>

typedef struct page Page;
Page * createPage(int sizeBytes, int number);
char insertAddress(Page * p, unsigned int addr,int offSet, char control);