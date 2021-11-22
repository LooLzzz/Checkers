#include "array.h"

Array array_new(int item_size)
{
    Array array = {
        .data = NULL,
        .length = 0,
        .item_size = item_size,
    };
    
    return array;
}

void array_push(Array *array, void *data)
{
    array->data = realloc(array->data, (array->length + 1) * array->item_size);
    memcpy((char*)array->data + array->length * array->item_size, data, array->item_size);
    
    array->length++;
}

void *array_get(Array *array, int index)
{
    return (char*)array->data + index * array->item_size;
}

void array_free(Array *array)
{
    free(array->data);
    
    array->data = NULL;
    array->length = 0;
}




