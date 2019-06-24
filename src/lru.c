#include <stdio.h>
#include "list.h"

typedef struct LRU lru;

struct LRU
{
    int *cache;
    int max_cap;
    ListHead *list;
};

/* 
void alocate_cache(int **cache, int n)
{
    cache = (void**)malloc(sizeof(void*)*n);
    for(int i = 0; i < n; i++)
        cache[i] = (int*)malloc(sizeof(int)*n);
} */

lru *createLRU(int capacity)
{
    lru *newLRU = (lru*)malloc(sizeof(lru));
    newLRU->cache = (int*)malloc(sizeof(int)*capacity);
    newLRU->max_cap = capacity;
    newLRU->list = createHead();
}


int get(lru *lru, int key) 
{
    if(lru->cache[key]!= NULL)
    {
        removeNode(key, lru->list);
        push_backNode(key, lru->list);
        return lru->cache[key];
    }
    return -1;
}

void put(int key, int value, lru *lru) 
{
    if(lru->list->size < lru->max_cap)
    {
        if(!lru->cache[key])
            push_backNode(key, lru->list);
        else
        {
            removeNode(key, lru->list);
            push_backNode(key, lru->list);
        }
        lru->cache[key] = value;
    }
    else
    {
        if(lru->cache[key])
        {
            removeNode(key, lru->list);
            push_backNode(key, lru->list);
            lru->cache[key] = value;
        }
        else
        {
            int old_key = lru->list->first;
            pop_frontNode(lru->list);
            lru->cache[key] = 0;
            push_backNode(key, lru->list);
        }
        lru->cache[key] = value;
    }
}