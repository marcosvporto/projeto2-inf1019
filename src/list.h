#include <stdio.h>

typedef struct ListNode ListNode;
typedef struct ListHead ListHead;


struct ListHead
{
    ListNode *first;
    ListNode *end;
    int size;
};

struct ListNode
{
    int val;
    ListNode *prev;
    ListNode *next;
};


ListHead *createHead();
ListNode *createNodea(int x, ListNode *prev, ListNode *next);
void removeNode(int key, ListHead *list);
void push_backNode(int val, ListHead* list);
void pop_frontNode(ListHead *list);
