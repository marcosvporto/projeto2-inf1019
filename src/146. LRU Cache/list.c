#include <stdio.h>
#include <list.h>


ListHead *createHead()
{
    ListHead *head = (ListHead*)malloc(sizeof(ListHead));
    head->first = NULL;
    head->end = NULL;
    head->size = 0;
}

ListNode *createNode(int x, ListNode *prev, ListNode *next)
{
    ListNode *n = (ListNode*)malloc(sizeof(ListNode));
    n->val = x;
    n->prev = prev;
    n->next = next;
}

void remove(int key, ListHead *list)
{
    ListNode *cur1 = list->first;
    ListNode *cur2 = list->end;
    int i = 0, j = list->size-1;

    for(;i <= j; i++)
    {
        if(cur1->val == key)
        {
            cur1->prev->next = cur1->next;
            free(cur1);
            return;
        }
        if(cur2->val == key)
        {
            cur2->prev->next = cur2->next;
            free(cur2);
            return;
        }
        cur1 = cur1->next;
        cur2 = cur2->prev;
        j--;
    }
}

void push_backNode(int val, ListHead* list)
{
    ListNode *node = createNode(val, list->end, NULL);
    list->end->next = node;
    list->end = node;
}

void pop_frontNode(ListHead *list)
{
    ListNode *temp = list->end;
    list->end->prev->next = NULL;
    list->end = list->end->prev;
    free(temp);
}