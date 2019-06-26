#include <stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<string.h>
#include"page.h"

typedef struct swapArea SwapArea;

typedef struct swapPage SwapPage;


Page * getPage(int pageNumber);