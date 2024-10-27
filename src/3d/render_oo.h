
#ifndef _GROOVER_RENDER_OO_H
#define _GROOVER_RENDER_OO_H

#include "image.h"
#include "scenedef.h"
#include "gfx_types.h"
#include "matrix.h"

void render_wireframe(struct Scene* scene, struct Image* img);

void render_signature(struct Scene* scene, struct Image* img);

void render_phong(struct Scene* scene, struct Image* img);

void render_gourad(struct Scene* scene, struct Image* img);

#endif /* _GROOVER_RENDER_OO_H */
