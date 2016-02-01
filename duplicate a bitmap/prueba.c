#include <stdio.h>
#include <stdlib.h>
#include "bm256.h"

int main(void) {
    bm256_t info;
    bm256_status_t status = bm256_create("girl.bmp", &info);
    
    if (status == OK) {
        printf("Size in bytes               : %d\n", info.size);
        printf("Total number of pixels      : %d\n", info.num_pixels);
        printf("Width in pixels             : %d\n", info.width);
        printf("Height in pixels            : %d\n", info.height);
        printf("Bits per pixel              : %d\n", info.bits_per_pixel);
        printf("Full file contents start at : %p\n", info.contents);
        printf("First pixel at              : %p\n", info.pixels);        
        bm256_dispose(&info);
        
    } else {
        fprintf(stderr, "Error (%d)\n", status);
    }
    
    return EXIT_SUCCESS;
}
