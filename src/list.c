#include "list.h"

List list_init(int max_length)
{
    List list = {
        .head       = NULL,
        .tail       = NULL,
        .max_length = max_length,
        .length     = 0,
    };

    return list;
}

ListNode *list_node_init(void *data, ListNode *prev, ListNode *next)
{
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    node->data     = data;
    node->prev     = prev;
    node->next     = next;

    return node;
}

ListNode *list_get(List *list, int index)
{
    ListNode *node = list->head;
    int i          = 0;

    // access head or tail
    if (index == 0)
        return list->head;
    else if (index == -1 || index >= list->length)
        return list->tail;

    // random access node
    while (node != NULL && i < index)
    {
        if (node->next == NULL)
            break;

        node = node->next;
        i++;
    }

    return node;
}

void list_insert(List *list, int index, void *data)
{
    ListNode *node, *newNode;
    void *tmp;

    // `max_length <= -1` means infinite length
    if (list->max_length > 0 && list->length == list->max_length)
    {
        // remove tail
        tmp = list_delete(list, list->length - 1);
        free(tmp);
    }

    if (list->length == 0)
    {
        list->head = list_node_init(data, NULL, NULL);
        list->tail = list->head;
    }
    else
    {
        if (index == 0)
        {
            // insert at head
            newNode          = list_node_init(data, NULL, list->head);
            list->head->prev = newNode;
            list->head       = newNode;
        }
        else if (index == list->length - 1)
        {
            // insert at tail
            newNode          = list_node_init(data, list->tail, NULL);
            list->tail->next = newNode;
            list->tail       = newNode;
        }
        else
        {
            // insert at index
            node             = list_get(list, index);
            newNode          = list_node_init(data, node->prev, node);
            node->prev->next = newNode;
            node->prev       = newNode;
        }
    }

    (list->length)++;
}

void list_push(List *list, void *data)
{
    return list_insert(list, 0, data);
}

void list_append(List *list, void *data)
{
    return list_insert(list, -1, data);
}

void *list_delete(List *list, int index)
{
    ListNode *node = list_get(list, index);
    void *res      = NULL;

    if (node)
    {
        if (node->prev)
            node->prev->next = node->next;
        else
            list->head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            list->tail = node->prev;

        (list->length)--;
        res = node->data;
        free(node);
    }

    return res;
}

void list_free(List *list)
{
    ListNode *node = list->head;
    ListNode *tmp;

    while (node)
    {
        tmp  = node;
        node = node->next;
        free(tmp->data);
        free(tmp);
    }

    list->head   = NULL;
    list->tail   = NULL;
    list->length = 0;
}
