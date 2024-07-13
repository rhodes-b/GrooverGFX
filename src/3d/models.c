
#include <stdlib.h>
#include "models.h"
#define M_TAU (M_PI * 2)


/*
class Sphere:
    """ Model of a sphere shape"""

    def intersect(self, ray, interval, info):
        """ returns a True iff ray intersects the sphere within the

        given time interval. The approriate intersection information
        is recorded into info, which is a Record containing:
          point: the point of intersection
          t: the time of the intersection
          normal: the surface normal at the point
          color: the color at the point.
        """
        a = ray.dir.mag2()
        if a == 0:
            return False
        rp_vec = ray.start - self.pos
        b = 2 * ray.dir.dot(rp_vec)
        c = rp_vec.mag2() - self.radius * self.radius
        discrim = b*b - 4*a*c
        if discrim < 0:
            return False
        t1 = (-b - sqrt(discrim)) / (2 * a)
        if t1 in interval:
            self._setinfo(ray, t1, info)
            return True
        t2 = (-b + sqrt(discrim)) / (2 * a)
        if t2 in interval:
            self._setinfo(ray, t2, info)
            return True
        return False

    def _setinfo(self, ray, t, info):
        """ helper method to fill in the info record """
        info.update(t=t, point=ray.point_at(t), color=self.color)
        info.normal = (info.point - self.pos).normalized()
*/


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

static void sphere_make_bands(struct Sphere* s) {
    // TODO: I think this is the correct amount?
    s->bands = malloc(s->nlat*s->nlong*sizeof(struct Point3F32));

    float cx = s->pos.x;
    float cy = s->pos.y;
    float cz = s->pos.z;

    float theta = M_PI_2;
    float dtheta = M_PI / (s->nlat+1);
    for(uint8_t i=0; i < s->nlat; i++) {
        theta += dtheta;
        float r = s->radius * cosf(theta);
        float y = s->radius * sinf(theta) + cy;

        struct Point2F32 pts[s->nlong];
        circ2d_points((struct Point2F32){cx, cz}, r, s->nlong, &pts[0]);
        for(uint8_t j=0; j < s->nlong; j++) {
            s->bands[i * s->nlong + j] = (struct Point3F32){pts[j].x, y, pts[j].y};
        }
    }
}

static struct Node* sphere_iter_polys(struct Sphere* s) {

    // TODO: verify this is correct way of looping over these

    struct Node* head = make_node();
    struct Node* curr = head;
    for(uint8_t i=0; i < s->nlong; i++) {
        struct Record r;
        struct Point3F32* pts = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 3);
        pts[0] = s->northpole;
        pts[1] = s->bands[i];
        pts[2] = s->bands[i+1];
        r.pts = pts;
        r.n_pts = 3;
        r.color = s->color;

        struct Node* n = make_node();
        curr->data = r;
        curr->next = n;
        curr = n;
    }

    for(uint8_t i=0; i < s->nlat-1; i++) {
        for(uint8_t j=0; j < s->nlong; j++) {
            struct Record r;
            struct Point3F32* pts = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 4);
            pts[0] = s->bands[i * s->nlat + j];
            pts[1] = s->bands[(i+1) * s->nlat + j];
            pts[2] = s->bands[(i+1) * s->nlat + (j+1)];
            pts[3] = s->bands[i * s->nlat + (j+1)];
            r.pts = pts;
            r.n_pts = 4;
            r.color = s->color;

            struct Node* n = make_node();
            curr->data = r;
            curr->next = n;
            curr = n;
        }
    }

    uint8_t last_band = s->nlat-1;
    for(uint8_t i=0; i < s->nlong; i++) {
        struct Record r;
        struct Point3F32* pts = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 3);
        pts[0] = s->southpole;
        pts[1] = s->bands[last_band + i];
        pts[2] = s->bands[last_band + i+1];
        r.pts = pts;
        r.n_pts = 3;
        r.color = s->color;

        struct Node* n = make_node();
        curr->data = r;
        curr->next = n;
        curr = n;
    }
    return head;
}

static void sphere_set_info(struct Sphere* s, struct Ray* r, float t, struct Record* info);
// TODO: impliment

static bool sphere_intersect(struct Sphere* s, struct Ray* r, struct Interval* i, struct Record* info) {
    return false;
}


void free_record(struct Record* r) {
    free(r->pts);
}

struct Sphere make_sphere(struct Point3F32 pos, float radius, struct Pixel color, uint8_t nlat, uint8_t nlong) {
    struct Sphere s = {0};
    s.pos = pos;
    s.radius = radius;
    s.color = color;
    s.nlat = nlat;
    s.nlong = nlong;
    s.axis = make_vec3(0., radius, 0.);
    s.northpole = vec3_add_pt(&s.axis, &s.pos);
    s.southpole = vec3_sub_pt(&s.axis, &s.pos);
    sphere_make_bands(&s);
    s.iter_polygons = sphere_iter_polys;
    s.intersect = sphere_intersect;
    return s;
}

void free_sphere(struct Sphere* s) {
    free(s->bands);
}
/*
for box

def _make_vertices(self):
        
def _in_rect(self, p, axis):

*/