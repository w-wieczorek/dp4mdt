#pragma once

#include <variant>
#include "read_data.h"
#include "mts_types.h"

typedef struct _node {
    std::variant<test, int> val;
    struct _node* left;
    struct _node* right;    
} node;

void free_tree(node* root);
void print_tree(node* root, int depth);
int height(node* root);
int tree_size(node* root);
int classify(node* root, const Table& dtable, int idx);