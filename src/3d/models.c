
#include <stdlib.h>
#include "models.h"
#define M_TAU (M_PI * 2)

static void circ2d_points(struct Point2F32 center, float radius, uint16_t segments, struct Point2F32* pts) {
    float dtheta = M_TAU / segments;
    float theta = 0.0;
    
    for(uint16_t i=0; i < segments; i++) {
        float x =  center.x + radius * cos(theta);
        float y =  center.y + radius * sin(theta);
        pts[i] = (struct Point2F32){ .x = x, .y = y };
        theta += dtheta;
    }
}

static void sphere_make_bands(struct Sphere* s, uint8_t nlat, uint8_t nlong) {
    // TODO: I think this is the correct amount?
    s->bands = malloc(nlat*nlong*sizeof(struct Point3F32));

    float cx = s->pos.x;
    float cy = s->pos.y;
    float cz = s->pos.z;

    float theta = M_PI_2;
    float dtheta = M_PI / (nlat+1);
    for(uint8_t i=0; i < nlat; i++) {
        theta += dtheta;
        float r = s->radius * cosf(theta);
        float y = s->radius * sinf(theta) + cy;

        struct Point2F32 pts[nlong];
        circ2d_points((struct Point2F32){cx, cz}, r, nlong, &pts[0]);
        for(uint8_t j=0; j < nlong; j++) {
            s->bands[i * nlong + j] = (struct Point3F32){pts[j].x, y, pts[j].y};
        }
    }
}

static void sphere_set_info(struct Sphere* s, struct Ray* r, float t, struct Record* info);

/*
for box

def _make_vertices(self):
        
def _in_rect(self, p, axis):

*/