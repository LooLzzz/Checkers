#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct _LIST_NODE
{
    void *data;
    struct _LIST_NODE *prev;
    struct _LIST_NODE *next;
} ListNode;

typedef struct _LIST
{
    ListNode *head;
    ListNode *tail;
    int max_length;
    int length;
} List;

List list_init(int max_length);
ListNode *list_node_init(void *data, ListNode *prev, ListNode *next);
/**
 * @return return node at index or tail node if index is an out of bounds positive number, otherwise return NULL
 */
ListNode *list_get(List *list, int index);
void list_insert(List *list, int index, void *data);
void *list_delete(List *list, int index);
void list_push(List *list, void *data);
void list_append(List *list, void *data);
void list_free(List *list);
