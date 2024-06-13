
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "image.h"

// TODO: const char* for fname?
static void _load_image(struct Image* img, char* fname) {
    // TODO:
}

static void _save_image(struct Image* img, char *fname) {
    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(fd < 0) {
        fprintf(stderr, "Could not open file for saving, err:%s\n", strerror(errno));
        return;
    }

    char buff[32] = {0};
    snprintf(buff, sizeof(buff), "P6\n");
    write(fd, buff, strlen(buff));
    
    snprintf(buff, sizeof(buff), "%d %d\n", img->width, img->height);
    write(fd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "255\n");
    write(fd, buff, sizeof("255\n"));

    size_t written_bytes = write(fd, img->pixels, img->width*img->height*3*sizeof(uint8_t));

    printf("Wrote %ld bytes\n", written_bytes);

    close(fd);
}

// TODO: static memory allocation?
// IE have a max array size in each image and warn if they are too big
struct Image make_image(uint16_t width, uint16_t height) {

    uint8_t* buff = (uint8_t*)malloc(width*height*3*sizeof(uint8_t));

    memset(buff, 255, width*height*3*sizeof(uint8_t));

    return (struct Image) {
        .pixels = buff,
        .width = width,
        .height = height,
        .load = _load_image,
        .save = _save_image,
    };

}

void free_image(struct Image* img) {
    free(img->pixels);
}
