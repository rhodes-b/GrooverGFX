
#include "models.h"

/*

void circle_points(struct PointF32 center, float radius, uint16_t segments, struct PointF32* pts) {
    float dtheta = M_TAU / segments;
    float theta = 0.0;
    
    for(uint16_t i=0; i < segments; i++) {
        float x =  center.x + radius * cos(theta);
        float y =  center.y + radius * sin(theta);
        pts[i] = (struct PointF32){ .x = x, .y = y };
        theta += dtheta;
    }
}

*/

static void sphere_make_bands(struct Sphere* s, uint8_t nlat, uint8_t nlong);
// TODO info...
static void sphere_set_info(struct Sphere* s, struct Ray* r, float t, void* info);

/*
for box

def _make_vertices(self):
        
def _in_rect(self, p, axis):

*/