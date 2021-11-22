#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objects.h"

/**
 * Array struct
 * @param length amount of items in the array
 * @param item_size size of each item in the array
 * @param data starting address of the array
 */
typedef struct _ARRAY
{
    int length;
    int item_size;
    void *data;
} Array;

Array array_new(int item_size);

/**
 * Clone given data and append it to the end of the array
 */
void array_push(Array *array, void *data);

/**
 * Retrieve item's pointer from a given array & index
 */
void *array_get(Array *array, int index);

void array_free(Array *array);
