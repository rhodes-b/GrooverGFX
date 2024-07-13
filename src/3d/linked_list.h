
#ifndef _GROOVER_LINKED_LIST_H
#define _GROOVER_LINKED_LIST_H

#include "gfx_types.h"
#include "ray3d.h"

struct Record {
    struct Point3F32* pts;
    uint16_t n_pts;
    struct Pixel color;
    struct Ray normal;
    float time;
    // ? uvn
    // ? texture
};

struct Node {
    struct Node* next;
    struct Record data;
};

struct Node* make_node();

void free_node(struct Node* n);

void free_nodes(struct Node* n);

#endif /* _GROOVER_LINKED_LIST_H */