
#include <render2d.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static void trans_pt(struct Render2d* render, struct PointF32 pt, struct PointF32* t_pt) {

    float arr[] = {pt.x, pt.y, 1};
    float res[3] = {0.0};

    matrix_apply(&render->transform, &arr[0], 3, &res[0]);

    t_pt->x = res[0];
    t_pt->y = res[1]; 

}
static void trans_pts(struct Render2d* render, struct PointF32* pts, uint16_t n_pts, struct PointF32* trans_pts) {
    for(uint16_t i=0; i < n_pts; i++) {
        trans_pt(render, pts[i], &trans_pts[i]);
    }
}

static struct Pixel get_color(struct Render2d* render) {
    return render->painter.color;
}

static void set_color(struct Render2d* render, struct Pixel c) {
    render->painter.color = c;
}

static void get_viewport(struct Render2d* render, struct PointI16* p1, struct PointI16* p2) {
    *p1 = render->painter.viewport[0];
    *p2 = render->painter.viewport[1];
}

static void loadview(struct Render2d* render, struct PointI16 window[2], struct PointI16 viewport[2]) {
    render->window[0] = window[0];
    render->window[1] = window[1];
    if(viewport != NULL) {
        render->painter.viewport[0] = viewport[0];
        render->painter.viewport[1] = viewport[1];
    }
    uint16_t vleft = render->painter.viewport[0].x;
    uint16_t vbottom = render->painter.viewport[0].y;
    uint16_t vright = render->painter.viewport[1].x;
    uint16_t vtop = render->painter.viewport[1].y;

    free_matrix(&render->transform);

    render->transform = matrix_window(
        (struct PointF32[]){{window[0].x, window[0].y}, {window[1].x, window[1].y}}, 
        (struct PointF32[]){{vleft - 0.5, vbottom - 0.5}, {vright + 0.5, vtop + 0.5}}
    );
}

static void push_matrix(struct Render2d* render, struct Matrix* m) {
    render->trans_stack[render->n_transforms++] = m;
}

static struct Matrix* pop_matrix(struct Render2d* render) {
    if(render->n_transforms == 0) {
        fprintf(stderr, "No matrix transforms to pop");
        exit(1);
    }
    render->n_transforms--;
    return render->trans_stack[render->n_transforms+1];
}

static void ren2d_scale(struct Render2d* render, float sx, float sy) {
    struct Matrix tmp = scale_xy(sx, sy);
    struct Matrix res = matrix_mul(&render->transform, &tmp);
    free_matrix(&tmp);
    free_matrix(&render->transform);
    render->transform = res;
}

static void ren2d_rotate(struct Render2d* render, float angle) {
    struct Matrix tmp = rotate_degrees_xy(angle);
    struct Matrix res = matrix_mul(&render->transform, &tmp);
    free_matrix(&tmp);
    free_matrix(&render->transform);
    render->transform = res;
}

static void ren2d_translate(struct Render2d* render, float dx, float dy) {
    struct Matrix tmp = translate_xy(dx, dy);
    struct Matrix res = matrix_mul(&render->transform, &tmp);
    free_matrix(&tmp);
    free_matrix(&render->transform);
    render->transform = res;
}

static void ren2d_shear(struct Render2d* render, float sx, float sy) {
    struct Matrix tmp = shear_xy(sx, sy);
    struct Matrix res = matrix_mul(&render->transform, &tmp);
    free_matrix(&tmp);
    free_matrix(&render->transform);
    render->transform = res;
}

static void ren2d_reflect_x(struct Render2d* render) {
    struct Matrix tmp = reflect_x();
    struct Matrix res = matrix_mul(&render->transform, &tmp);
    free_matrix(&tmp);
    free_matrix(&render->transform);
    render->transform = res;
}

static void ren2d_reflect_y(struct Render2d* render) {
    struct Matrix tmp = reflect_y();
    struct Matrix res = matrix_mul(&render->transform, &tmp);
    free_matrix(&tmp);
    free_matrix(&render->transform);
    render->transform = res;
}

void ren2d_point(struct Render2d* render, struct PointF32 pt) {
    struct PointF32 t_pt = {0};
    trans_pt(render, pt, &t_pt);
    render->painter.draw_point(&render->painter, t_pt);
}

void ren2d_line(struct Render2d* render, struct PointF32 p1, struct PointF32 p2) {
    struct PointF32 t_pts[2] = {0};
    trans_pts(render, (struct PointF32[]){p1, p2}, 2, &t_pts[0]);
    render->painter.draw_line(&render->painter, t_pts[0], t_pts[1]);
}

void ren2d_lines(struct Render2d* render, struct PointF32* pts, uint16_t n_pts) {
    struct PointF32 t_pts[n_pts];
    trans_pts(render, pts, n_pts, &t_pts[0]);
    render->painter.draw_lines(&render->painter, &t_pts[0], n_pts);
}

void ren2d_polygon(struct Render2d* render, struct PointF32* vertices, uint16_t n_vertices) {
    render->lines(render, vertices, n_vertices);
    render->line(render, vertices[0], vertices[n_vertices-1]);
}

void ren2d_circle(struct Render2d* render, struct PointF32 center, float radius, uint16_t segments) {
   struct PointF32 pts[segments];
   struct PointF32 t_pts[segments];
   circle_points(center, radius, segments, &pts[0]);
   trans_pts(render, &pts[0], segments, &t_pts[0]);
   render->painter.draw_polygon(&render->painter, t_pts, segments);
}

void ren2d_filled_triangle(struct Render2d* render, struct PointF32 p1, struct PointF32 p2, struct PointF32 p3) {
    struct PointF32 t_pts[3];
    trans_pts(render, (struct PointF32[]){p1, p2, p3}, 3, &t_pts[0]);
    render->painter.draw_filled_triangle(&render->painter, t_pts[0], t_pts[1], t_pts[2]);
}

void ren2d_filled_polygon(struct Render2d* render, struct PointF32* vertices, uint16_t n_vertices) {
    struct PointF32 t_pts[n_vertices];
    trans_pts(render, vertices, n_vertices, &t_pts[0]);
    render->painter.draw_filled_polygon(&render->painter, &t_pts[0], n_vertices);
}

void ren2d_filled_circle(struct Render2d* render, struct PointF32 center, float radius, uint16_t segments) {
    struct PointF32 pts[segments];
    struct PointF32 t_pts[segments];
    circle_points(center, radius, segments, &pts[0]);
    trans_pts(render, &pts[0], segments, &t_pts[0]);
    render->painter.draw_filled_polygon(&render->painter, t_pts, segments);
}

struct Render2d make_render2d(uint16_t width, uint16_t height) {
    struct Image img = make_image(width, height);
    struct Painter painter = make_painter(&img);

    struct Render2d render = {
        .image = img,
        .painter = painter,
        .transform = make_idenity_matrix(3),
        .n_transforms = 0,
        .window = {{0, 0}, {width-1, height-1}},

        .get_color = get_color,
        .set_color = set_color,
        .get_viewport = get_viewport,
        .loadview = loadview,
        .push_matrix = push_matrix,
        .pop_matrix = pop_matrix,
        .scale = ren2d_scale,
        .rotate = ren2d_rotate,
        .translate = ren2d_translate,
        .shear = ren2d_shear,
        .reflect_x = ren2d_reflect_x,
        .reflect_y = ren2d_reflect_y,
        .point = ren2d_point,
        .line = ren2d_line,
        .lines = ren2d_lines,
        .polygon = ren2d_polygon,
        .circle = ren2d_circle,
        .filled_triangle = ren2d_filled_triangle,
        .filled_polygon = ren2d_filled_polygon,
        .filled_circle = ren2d_filled_circle,
    };

    render.loadview(&render, &render.window[0], NULL);
    return render;
}   
