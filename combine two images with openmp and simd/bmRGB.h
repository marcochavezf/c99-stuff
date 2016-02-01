/*
 +­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 
 | Nombre:    Marco Antonio Chávez Fuentes
 | Fecha:     14 de mayo, 2010.
 +­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 
 */

#ifndef _BMRGB_H_
#define _BMRGB_H_

#include <stdint.h>

#define WIDTH_OFFSET            18
#define HEIGHT_OFFSET           22
#define BITS_PER_PIXEL_OFFSET   28
#define COMPRESSION_OFFSET      30
#define PIXEL_OFFSET            54
#define NO_COMPRESSION          0
#define EXPECTED_BITS_PER_PIXEL 24

enum bmRGB_status { 
    OK, 
    FILE_ERROR, 
    BAD_BMP_FILE_ERROR, 
    OUT_OF_MEMORY_ERROR, 
    UNSUPPORTED_FILE_FORMAT_ERROR };

struct bmRGB_tag {
    uint32_t size;
    uint32_t num_pixels;
    uint32_t width;
    uint32_t height;
    uint16_t bits_per_pixel; 
    uint8_t* contents;
    uint8_t* pixels; 
};

typedef struct bmRGB_tag    bmRGB_t;
typedef enum bmRGB_status   bmRGB_status_t; 

extern bmRGB_status_t bmRGB_create(const char* name, bmRGB_t* obj);
extern bmRGB_status_t bmRGB_dispose(bmRGB_t* obj);

#endif

