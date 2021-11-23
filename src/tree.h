#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct _NODE
{
    void *data;
    struct _NODE *parent;
    struct _NODE *left;
    struct _NODE *right;
} Node;

Node *tree_init_root(void *data);
Node *tree_init_node(Node *parent, void *data);

void tree_free(Node *root);
void tree_insert(Node *parent, void *data);
