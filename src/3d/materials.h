
#ifndef GROOVER_MATERIAL_H
#define GROOVER_MATERIAL_H

#include "gfx_types.h"
#include <stdbool.h>

struct Material {
    struct Pixel diffuse;
    struct Pixel ambient;
    struct Pixel specular;
    float shininess;
    bool reflect;
};


#define MATERIALS_LIST \
    X(BLACK_PLASTIC, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 32.0, false)                                                   \
    X(GREEN_PLASTIC, 0.0, 0.6, 0.0, 0.0, 0.3, 0.0, 0.6, 0.8, 0.6, 32.0, false)                                                   \
    X(RED_PLASTIC, 0.6, 0.0, 0.0, 0.3, 0.0, 0.0, 0.8, 0.6, 0.6, 32.0, false)                                                     \
    X(BRASS, 0.780392, 0.568627, 0.113725, 0.329412, 0.223529, 0.027451, 0.992157, 0.941176, 0.807843, 27.8974, false)           \
    X(COPPER, 0.7038, 0.27048, 0.0828, 0.19125, 0.0735, 0.0225, 0.256777, 0.137622, 0.086014, 12.8, false)                       \
    X(GOLD, 0.75164, 0.60648, 0.22648, 0.24725, 0.1995, 0.0745, 0.628281, 0.555802, 0.366065, 51.2, false)                       \
    X(SILVER, 0.50754, 0.50754, 0.50754, 0.19225, 0.19225, 0.19225, 0.508273, 0.508273, 0.508273, 51.2, false)

#define X(mat, ...) \
    extern struct Material mat;
    MATERIALS_LIST
#undef X

// unfortunatly we have to do this
void init_materials();

/*

BLACK_PLASTIC = Material(diffuse=(.01, .01, .01), ambient=(0, 0, 0),
                         specular=(.5, .5, .5), shininess=32)

GREEN_PLASTIC = Material(diffuse=(0, .6, 0), ambient=(0, .3, 0),
                         specular=(.6, .8, .6), shininess=32)

RED_PLASTIC = Material(diffuse=(.6, 0, 0), ambient=(.3, 0, 0),
                       specular=(.8, .6, .6), shininess=32)

BRASS = Material((.780392, .568627, .113725), (.329412, .223529, .027451),
                 (.992157, .941176, .807843), 27.8974)

COPPER = Material((.7038, .27048, .0828), (.19125, .0735, .0225),
                  (.256777, .137622, .086014), 12.8)

GOLD = Material((.75164, .60648, .22648), (.24725, .1995, .0745),
                (.628281, .555802, .366065), 51.2)

SILVER = Material((.50754,)*3, (.19225,)*3, (.508273,)*3, 51.2)

*/

#endif /* GROOVER_MATERIAL_H */
