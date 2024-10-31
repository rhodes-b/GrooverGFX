
#include "materials.h"


#define X(mat)                 \
    struct Material mat = {0}; \
    MATERIALS_LIST
#undef X

/*
    struct Pixel diffuse;
    struct Pixel ambient;
    struct Pixel specular;
    float shininess;
    bool reflect;
 */

// unfortunatly we have to do this
void init_materials() {
    #define X(mat, d1, d2, d3, a1, a2, a3, s1, s2, s3, shininess, reflect) \
        mat.diffuse = make_pixel(d1, d2, d3);                              \
        mat.ambient = make_pixel(a1, a2, a3);                              \
        mat.specular = make_pixel(s1, s2, s3);                             \
        mat.shininess = shininess;                                         \
        mat.reflect = reflect;                                             
    #undef X
};

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
