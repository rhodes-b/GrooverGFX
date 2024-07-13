
#ifndef _GROOVER_LINKED_LIST_H
#define _GROOVER_LINKED_LIST_H

#include "models.h"

struct Node {
    struct Node* next;
    struct Record data;
};

struct Node* make_node();

void free_node(struct Node* n);

void free_nodes(struct Node* n);

#endif /* _GROOVER_LINKED_LIST_H */