/*
 +­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 
 | Nombre:    Marco Antonio Chávez Fuentes
 | Fecha:     14 de mayo, 2010.
 +­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 
 */


#include <stdio.h>
#include <stdlib.h>
#include "bmRGB.h"

bmRGB_status_t bmRGB_create(const char* name, bmRGB_t* obj) {
    
    FILE* file = fopen(name, "rb");
    
    if (file == NULL) {
        return FILE_ERROR;
    }
    
    int b0 = fgetc(file);
    int b1 = fgetc(file);
    
    if (b0 != 'B' || b1 != 'M') {
        fclose(file);
        return BAD_BMP_FILE_ERROR;
    }
    
    fread(&(obj -> size), sizeof(obj -> size), 1, file);
    
    
    obj -> contents = malloc(obj -> size);
    
    if (obj -> contents == NULL) {
        fclose(file);
        return OUT_OF_MEMORY_ERROR;
    }
    
    rewind(file);
    size_t bytes_read = fread(obj -> contents, sizeof(uint8_t), obj -> size, file);
    fclose(file);
    
    if (bytes_read != obj -> size) {
        free(obj -> contents);
        obj -> contents = NULL;
        return BAD_BMP_FILE_ERROR;
    }
    
    uint32_t compression = *((uint32_t*) (obj -> contents + COMPRESSION_OFFSET));
    obj -> bits_per_pixel = *((uint16_t*) (obj -> contents + BITS_PER_PIXEL_OFFSET));
    
    if (compression != NO_COMPRESSION 
        || obj -> bits_per_pixel != EXPECTED_BITS_PER_PIXEL) {
        free(obj -> contents);
        obj -> contents = NULL;
        return UNSUPPORTED_FILE_FORMAT_ERROR;
    }
            
    obj -> num_pixels = obj -> size - PIXEL_OFFSET;
    obj -> width = *((uint32_t*) (obj -> contents + WIDTH_OFFSET));
    obj -> height = *((uint32_t*) (obj -> contents + HEIGHT_OFFSET));    
    obj -> pixels = obj -> contents + PIXEL_OFFSET;
    
    return OK;
}

bmRGB_status_t bmRGB_dispose(bmRGB_t* obj) {
    free(obj -> contents);  
    obj -> contents = NULL;
    obj -> pixels = NULL;
    return OK;
}




