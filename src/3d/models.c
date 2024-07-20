
#include <stdlib.h>
#include <math.h>
#include "models.h"
#include <math3d.h>
#define M_TAU (M_PI * 2)


static void circ2d_points(struct Point2F32 center, float radius, uint16_t segments, struct Point2F32* pts) {
    float dtheta = M_TAU / segments;
    
    float x = center.x;
    float y = center.y;

    for(uint16_t i=0; i < segments; i++) {
        pts[i] = (struct Point2F32){
            .x = (radius * cos(i * dtheta) + x),
            .y = (radius * sin(i * dtheta) + y),
        };
    }
    pts[segments] = pts[0];
}

static inline uint16_t get_band(struct Sphere* s, uint16_t index) {
    return index * (s->nlong+1);
}

static void sphere_make_bands(struct Sphere* s) {
    // leaked here?
    s->bands = malloc(s->nlat*(s->nlong+1)*sizeof(struct Point3F32));

    float cx = s->pos.x, cy = s->pos.y, cz = s->pos.z;

    float theta = M_PI_2;
    float dtheta = M_PI / (s->nlat+1);
    for(uint8_t i=0; i < s->nlat; i++) {
        theta += dtheta;
        float r = s->radius * cosf(theta);
        float y = s->radius * sinf(theta) + cy;

        struct Point2F32 pts[s->nlong+1];
        circ2d_points((struct Point2F32){cx, cz}, r, s->nlong, &pts[0]);
        for(uint8_t j=0; j < s->nlong+1; j++) {
            s->bands[get_band(s, i) + j] = (struct Point3F32){pts[j].x, y, pts[j].y};
        }
    }
}

static struct Node* sphere_iter_polys(struct Sphere* s) {
    // indirectly lost below?
    struct Node* head = make_node();
    struct Node* curr = head;
    for(uint8_t i=0; i < s->nlong; i++) {
        struct Record r;
        // leaked here?
        struct Point3F32* pts = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 3);
        pts[0] = s->northpole;
        pts[1] = s->bands[i];
        pts[2] = s->bands[i+1];
        r.pts = pts;
        r.n_pts = 3;
        r.color = s->color;

        // leaked here?
        struct Node* n = make_node();
        curr->data = r;
        curr->next = n;
        curr = n;
    }

    for(uint16_t i=0; i < s->nlat-1; i++) {
        for(uint16_t j=0; j < s->nlong; j++) {
            struct Record r;
            // leaked here?
            struct Point3F32* pts = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 4);
            pts[0] = s->bands[get_band(s, i) + j];
            pts[1] = s->bands[get_band(s, i+1) + j];
            pts[2] = s->bands[get_band(s, i+1) + (j+1)];
            pts[3] = s->bands[get_band(s, i) + (j+1)];

            r.pts = pts;
            r.n_pts = 4;

            // leaked here?
            struct Node* n = make_node();
            curr->data = r;
            curr->next = n;
            curr = n;
        }
    }

    uint8_t last_band = s->nlat-1;
    for(uint8_t i=0; i < s->nlong; i++) {
        struct Record r;
        // Valgrind says memory gets leaked here (alot) why?
        struct Point3F32* pts = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 3);
        pts[0] = s->southpole;
        pts[1] = s->bands[get_band(s, last_band) + i];
        pts[2] = s->bands[get_band(s, last_band) + i+1];
        r.pts = pts;
        r.n_pts = 3;
        r.color = s->color;

        curr->data = r;
        // leaked here?
        struct Node* n = make_node();
        curr->next = n;
        curr = n;
    }
    // make sure we clean the last empty node
    // is doubly linked worth for just this case?
    struct Node* tmp = curr;
    curr = head;
    while(curr->next != tmp) {
        curr = curr->next;
    }
    free_node(tmp);
    curr->next = NULL;
    return head;
}

static void sphere_set_info(struct Sphere* s, struct Ray* r, float t, struct Record* info) {
    info->time = t;
    if(info->pts != NULL) {
        free(info->pts);
    }
    info->pts = (struct Point3F32*)malloc(sizeof(struct Point3F32));
    struct Point3F32 pt = r->point_at(r, t);
    info->pts[0].x = pt.x;
    info->pts[0].y = pt.y;
    info->pts[0].z = pt.z;

    info->n_pts = 1;
    info->color = s->color;
    struct Vec3 tmp = point3F32_sub(&info->pts[0], &s->pos);
    info->normal = vec3_normalized(&tmp);
}

static bool sphere_intersect(struct Sphere* s, struct Ray* r, struct Interval* i, struct Record* info) {
    float a = vec3_mag_sq(&r->dir);
    if(a == 0) {
        return false;
    }
    struct Vec3 rp_vec = point3F32_sub(&r->start, &s->pos);
    float b = 2 * vec3_dot(&r->dir, &rp_vec);
    float c = vec3_mag_sq(&rp_vec) - s->radius * s->radius;
    float discrim = b*b - 4*a*c;
    if(discrim < 0) {
        return false;
    }
    float t1 = (-b - sqrt(discrim)) / (2 * a);
    if(i->contains(i, t1)) {
        sphere_set_info(s, r, t1, info);
        return true;
    }
    float t2 = (-b + sqrt(discrim)) / (2 * a);
    if(i->contains(i, t2)) {
        sphere_set_info(s, r, t2, info);
        return true;
    }
    return false;
}

static void make_planes(struct Box* b) {
    float sx = b->size.x * 0.5, sy = b->size.y * 0.5, sz = b->size.z * 0.5;
    float cx = b->pos.x, cy = b->pos.y, cz = b->pos.z;
    b->planes[0] = (struct Point2F32){cx - sx, cx + sx};
    b->planes[1] = (struct Point2F32){cy - sy, cy + sy};
    b->planes[2] = (struct Point2F32){cz - sz, cz + sz}; // -z is closer
}

static struct Node* box_iter_polys(struct Box* b) {
    struct Node* head = make_node();
    struct Node* curr = head;
    struct Point3F32* pts1 = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 4);
    struct Point3F32* pts2 = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 4);
    struct Point3F32* pts3 = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 4);
    struct Point3F32* pts4 = (struct Point3F32*)malloc(sizeof(struct Point3F32) * 4);

    struct Point2F32 x_p = b->planes[0], y_p = b->planes[1], z_p = b->planes[2];

    pts1[0] = (struct Point3F32){x_p.x, y_p.x, z_p.y};
    pts1[1] = (struct Point3F32){x_p.y, y_p.x, z_p.y};
    pts1[2] = (struct Point3F32){x_p.y, y_p.y, z_p.y};
    pts1[3] = (struct Point3F32){x_p.x, y_p.y, z_p.y};

    struct Node* next = make_node();
    curr->data = (struct Record){pts1, 4, b->color};
    curr->next = next;
    curr = next;

    pts2[0] = (struct Point3F32){x_p.x, y_p.x, z_p.x};
    pts2[1] = (struct Point3F32){x_p.y, y_p.x, z_p.x};
    pts2[2] = (struct Point3F32){x_p.y, y_p.y, z_p.x};
    pts2[3] = (struct Point3F32){x_p.x, y_p.y, z_p.x};

    next = make_node();
    curr->data = (struct Record){pts2, 4, b->color};
    curr->next = next;
    curr = next;

    pts3[0] = (struct Point3F32){x_p.x, y_p.x, z_p.y};
    pts3[1] = (struct Point3F32){x_p.x, y_p.x, z_p.x};
    pts3[2] = (struct Point3F32){x_p.x, y_p.y, z_p.x};
    pts3[3] = (struct Point3F32){x_p.x, y_p.y, z_p.y};

    next = make_node();
    curr->data = (struct Record){pts3, 4, b->color};
    curr->next = next;
    curr = next;

    pts4[0] = (struct Point3F32){x_p.y, y_p.x, z_p.y};
    pts4[1] = (struct Point3F32){x_p.y, y_p.x, z_p.x};
    pts4[2] = (struct Point3F32){x_p.y, y_p.y, z_p.x};
    pts4[3] = (struct Point3F32){x_p.y, y_p.y, z_p.y};

    next = make_node();
    curr->data = (struct Record){pts4, 4, b->color};
    curr->next = next;
    curr = next;

    return head;
}

static bool in_rect(struct Box* b, struct Point3F32* p, uint8_t axis) {
    // loop over all planes
    for(uint8_t a=0; a < 3; a++) {
        if(a == axis) {
            continue;
        }
        float low = b->planes[a].x, high = b->planes[a].y;
        float xy = a == 0 ? p->x : p->y;
        if(!((low <= xy) && (xy <= high))) {
            return false;
        }
    }
    return true;
}

/*
    hit = True
    interval.high = t
    info.update(t=t, point=ray.point_at(t), color=self.color)
    info.normal = Vector((0, 0, 0))
    info.normal[a] = -1.0 if lh == 0 else 1
*/

static bool box_intersect(struct Box* b, struct Ray* r, struct Interval* i, struct Record* info) {
    bool hit = false;
    // loop over all planes
    for(uint8_t a=0; a < 3; a++) {
        if(r->dir.get_pos(&r->dir, a) == 0.00) {
            continue;
        }
        // low and high
        for(uint8_t lh=0; lh < 2; lh++) {
            float plane_xy = lh == 0 ? b->planes[a].x : b->planes[a].y;
            float ray_start_xy = lh == 0 ? r->start.x : r->start.y;
            float ray_dir_xy = lh == 0 ? r->dir.x : r->dir.y;
            float t = (plane_xy - ray_start_xy) / ray_dir_xy;
            if(!i->contains(i, t)) {
                continue;
            }
            struct Point3F32 tmp = r->point_at(r, t);
            if(in_rect(b, &tmp, a)) {
                hit = true;
                i->high = t;
                info->time = t;
                info->pts = (struct Point3F32*)malloc(sizeof(struct Point3F32));
                *info->pts = tmp;
                info->n_pts = 1;
                info->normal = make_vec3(0, 0, 0);
                info->normal.set_pos(&info->normal, a, (lh == 0 ? -1. : 1.));
            }
        }
    }
    return hit;
}

static void group_add(struct Group* g, struct Shape* model) {
    g->objects[g->n_objects++] = *model;
}

static struct Node* update_node(struct Shape* s) {
    switch (s->shape_type) {
        case SPHERE:
            return s->shape.s.iter_polygons(&s->shape.s);
            break;
        case BOX:
            return s->shape.b.iter_polygons(&s->shape.b);
            break;
        default:
            // unreachable
            return NULL;
            break;
    }
}

static struct Node* group_iter_polys(struct Group* g) {
    // No data in head but each shapes head has data
    struct Node* head = NULL;
    struct Shape s = g->objects[0];
    head = update_node(&s);
    struct Node* curr = head;
    // get to end of iter
    while(curr->next != NULL) {
        curr = curr->next;
    }

    for(uint8_t i=1; i < g->n_objects; i++) {
        struct Shape s = g->objects[i];
        struct Node* iter = update_node(&s);
        curr->next = iter;
        curr = iter;
        // get to end of iter
        while(curr->next != NULL) {
            curr = curr->next;
        }
    }
    return head;
}

static bool group_intersect(struct Group* g, struct Ray* r, struct Interval* i, struct Record* info) {
    bool hit = false;
    for(uint8_t index=0; index < g->n_objects; index++) {
        struct Shape s = g->objects[index];
        switch (s.shape_type) {
            case SPHERE:
                if(s.shape.s.intersect(&s.shape.s, r, i, info)) {
                    hit = true;
                    i->high = info->time;
                }
                break;
            case BOX:
                if(s.shape.b.intersect(&s.shape.b, r, i, info)) {
                    hit = true;
                    i->high = info->time;
                }
                break;
            default:
                // unreachable
                break;
        }
    }
    return hit;
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

struct Box make_box(struct Point3F32 pos, struct Vec3 size, struct Pixel color) {
    struct Box b = {0};
    b.pos = pos;
    b.size = size;
    b.color = color;
    make_planes(&b);
    b.iter_polygons = box_iter_polys;
    b.intersect = box_intersect;
    return b;
}

struct Group make_group() {
    return (struct Group) {
        .add = group_add,
        .iter_polygons = group_iter_polys,
        .intersect = group_intersect,
    };
}

void free_sphere(struct Sphere* s) {
    free(s->bands);
}

void free_record(struct Record* r) {
    free(r->pts);
}
