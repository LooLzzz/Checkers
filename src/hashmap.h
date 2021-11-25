#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "list.h"
#include "objects.h"
#include "zobrist.h"

#define HASHMAP_SIZE 512

typedef struct _HASHMAP_ENTRY
{
    uint64_t key;
    void *value;
} HashmapEntry;

typedef struct _HASHMAP
{
    List buckets[HASHMAP_SIZE];
} Hashmap;

// Create a new hashmap
Hashmap hashmap_init();

// Free a hashmap
void hashmap_free(Hashmap *map);

// Add a value to the hashmap
void hashmap_put(Hashmap *map, uint64_t key, void *value);

// Get a value from the hashmap
void *hashmap_get(Hashmap *map, uint64_t key);
