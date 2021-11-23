#include "tree.h"

Node *tree_init_root(void *data)
{
    return tree_init_node(NULL, data);
}

Node *tree_init_node(Node *parent, void *data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
        return NULL;

    new_node->data   = data;
    new_node->parent = parent;
    new_node->left   = NULL;
    new_node->right  = NULL;
    return new_node;
}

void tree_free(Node *root)
{
    if (root == NULL)
        return;
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

void tree_insert(Node *parent, void *data)
{
    if (parent == NULL)
        return;
    if (parent->data == NULL)
        parent->data = data;
    else if (parent->left == NULL)
        parent->left = tree_init_node(parent, data);
    else if (parent->right == NULL)
        parent->right = tree_init_node(parent, data);
    else
        tree_insert(parent->left, data);
}
