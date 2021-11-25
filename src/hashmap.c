#include "hashmap.h"

Hashmap hashmap_init()
{
    int i;
    Hashmap map;

    for (i = 0; i < HASHMAP_SIZE; i++)
        map.buckets[i] = list_init(5);

    return map;
}

void hashmap_put(Hashmap *map, uint64_t key, void *value)
{
    int i;
    List *list;
    HashmapEntry *entry;
    void *tmp;

    i    = key % HASHMAP_SIZE;
    list = &map->buckets[i];

    // dont push if item is already present
    tmp = hashmap_get(map, key);
    if (tmp)
        return;

    // push item
    entry        = (HashmapEntry *)malloc(sizeof(HashmapEntry));
    entry->key   = key;
    entry->value = value;
    list_push(list, entry);
}

void *hashmap_get(Hashmap *map, uint64_t key)
{
    int i;
    List *list;
    ListNode *node;
    HashmapEntry *entry;

    i    = key % HASHMAP_SIZE;
    list = &map->buckets[i];
    node = list->head;    

    while (node)
    {
        entry = (HashmapEntry *)node->data;
        if (entry->key == key)
            return entry->value;
        node = node->next;
    }

    return NULL;
}

void hashmap_free(Hashmap *map)
{
    int i;
    List *list;

    for (i = 0; i < HASHMAP_SIZE; i++)
    {
        list = &map->buckets[i];
        // node = list->head;
        // while (node)
        // {
        //     entry = (HashmapEntry *)node->data;
        //     free(entry);
        //     node = node->next;
        // }
        list_free(list);
    }
}