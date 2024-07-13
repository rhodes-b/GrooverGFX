
#include <stddef.h>
#include <stdlib.h>
#include "linked_list.h"

struct Node* make_node() {
    return (struct Node*)malloc(sizeof(struct Node));
}

void free_node(struct Node* n) {
    free(n);
}

// recursive way to free from current head
void free_nodes(struct Node* n) {
    // we are at tail free and chain up
    if(n->next == NULL) {
        free_node(n);
        return;
    }
    // we aren't at tail recurse down and free after we start chaining up
    free_nodes(n->next);
    free_node(n);
}
