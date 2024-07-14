
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "image.h"

#define PPM_HEADER "P6\n"

static void load_img(struct Image* img, char* fname) {
    int fd = open(fname, O_RDONLY);
    if(fd < 0) {
        fprintf(stderr, "Could not read file, err:%s\n", strerror(errno));
        return;
    }

    char buff[32] = {0};
    read(fd, buff, sizeof(PPM_HEADER)-1);
    if(strncmp(PPM_HEADER, buff, sizeof(PPM_HEADER)-1) != 0) {
        fprintf(stderr, "Not PPM P6 format got:%s\n", buff);
    }

    // read width
    read(fd, buff, sizeof("%d"));
    img->width = strtoul(buff, NULL, 0);
    // skip space
    read(fd, buff, 1); 
    // read height
    read(fd, buff, sizeof("%d"));
    img->height = strtoul(buff, NULL, 0);
    // skip new line
    read(fd, buff, 1);

    // check max is 255
    read(fd, buff, sizeof("255\n")-1);
    if(strncmp("255\n", buff, sizeof("255\n")-1) != 0) {
        fprintf(stderr, "Pixel range not 255 got:%s\n", buff);
    }

    // allocate pixel array and read in data
    free(img->pixels);
    img->pixels = (uint8_t*)malloc(img->width*img->height*3);;
    if(img->pixels == NULL) {
        fprintf(stderr, "Malloc failed? %s\n", strerror(errno));
    }

    size_t bytes_read = read(fd, img->pixels, img->width*img->height*3);
    if(bytes_read < img->width*img->height*3) {
        fprintf(stderr,
                "Could not read correct number of image bytes expected:%d, got:%ld",
                img->width*img->height*3,
                bytes_read
        );
    }

    close(fd);
}

static void save_img(struct Image* img, char *fname) {
    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(fd < 0) {
        fprintf(stderr, "Could not open file for saving, err:%s\n", strerror(errno));
        return;
    }

    char buff[32] = {0};
    // write PPM HEADEr
    snprintf(buff, sizeof(buff), PPM_HEADER);
    write(fd, buff, strlen(buff));
    
    // write width and height
    snprintf(buff, sizeof(buff), "%d %d\n", img->width, img->height);
    write(fd, buff, strlen(buff));

    // write pixel value
    snprintf(buff, sizeof(buff), "255\n");
    write(fd, buff, sizeof("255\n")-1);

    // write pixels to file
    size_t written_bytes = write(fd, img->pixels, img->width*img->height*3);
    if(written_bytes == -1) {
        fprintf(stderr, "Error while writing:%s\n", strerror(errno));
    }

    close(fd);
}

// TODO: compare when py.y == img->height in c vs python
// I think python is abusing negative indexing which we cant
static inline uint32_t locate_pos(struct Image* img, struct Point2I16 pt) {
    return ((img->height - 1 - pt.y) * 3 * img->width) + (pt.x * 3);
}

static void set_pix(struct Image* img, struct Point2I16 pt, struct Pixel pix) {
    // TODO: why does python version work with pt.y <= img->height (am I missing a -1 somewhere?)
    if((pt.x >= 0) && (pt.x <= img->width) && (pt.y >= 0) && (pt.y < img->height)) {
        uint32_t arr_pos = locate_pos(img, pt);
        img->pixels[arr_pos+0] = pix.r;
        img->pixels[arr_pos+1] = pix.g;
        img->pixels[arr_pos+2] = pix.b;
    }
}

static struct Pixel get_pix(struct Image* img, struct Point2I16 pt) {
    uint16_t arr_pos = locate_pos(img, pt);
    return (struct Pixel) {
        .r = img->pixels[arr_pos+0],
        .g = img->pixels[arr_pos+1],
        .b = img->pixels[arr_pos+2],
    };
}

struct Image make_image(uint16_t width, uint16_t height) {
    // allocate memory for pixels
    uint8_t* buff = (uint8_t*)malloc(width*height*3);
    memset(buff, 255, width*height*3);
    // return image struct
    return (struct Image) {
        .pixels = buff,
        .width = width,
        .height = height,
        .get_pixel = get_pix,
        .set_pixel = set_pix,
        .load = load_img,
        .save = save_img,
    };
}

void free_image(struct Image* img) {
    free(img->pixels);
}
