
#include "render_oo.h"
#include "matrix.h"

struct FrameBuffer {
    struct Image img;
    struct Point2I16 window[2];

    struct Matrix transform;

    void (*draw_line)(struct FrameBuffer* fb, struct Point2F32 p1, struct Point2F32 p2);
    void (*draw_polygon)(struct FrameBuffer* fb, struct Point2F32* vertices, uint16_t n_vertices);
    struct Point2F32 (*trans_pt)(struct FrameBuffer* fb, struct Point3F32 p);
};

static struct FrameBuffer make_framebuffer() {
    (*NULL);
    return (struct FrameBuffer) {
        0
    };
}