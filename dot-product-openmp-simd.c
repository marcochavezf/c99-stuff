/*
 * Dot product using OpenMP and SIMD directives.
 * Marco Antonio Chavez
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <omp.h>
#include <x86intrin.h>

#define array_size(a) (sizeof(a)/sizeof(a[0]))

int producto_punto(size_t tamano, uint16_t* a, uint16_t* b) {

	int p_punto =0;
	for(int i=0; i<tamano; i++){
		p_punto += a[i]*b[i];	
	}	

    return p_punto;
}

int producto_punto_openmp(size_t tamano, uint16_t* a, uint16_t* b) {
    
	int p_punto =0;
	#pragma omp parallel for reduction(+:p_punto)
	for(int i=0; i<tamano; i++){
		p_punto += a[i]*b[i];
	}	
    return p_punto;
}

int producto_punto_simd(size_t tamano, uint16_t* a, uint16_t* b) {
    
	__m128i sumatoria = _mm_set1_epi32 (0);
	
	for(int i=0; i<tamano; i+=8){
		
		__m128i x_16b = _mm_loadu_si128((__m128i*)(a+i));
		__m128i y_16b = _mm_loadu_si128((__m128i*)(b+i));

		__m128i producto_32b = _mm_madd_epi16 (x_16b, y_16b);
		
		sumatoria = _mm_add_epi32 (sumatoria, producto_32b);
	}
	
	uint32_t* pp = malloc(sizeof(uint32_t)*4);
	_mm_storeu_si128 ((__m128i*)pp, sumatoria);
	
	int p_punto =0;
	for(int i=0; i<4; i++){
		p_punto += pp[i];
	}

    return p_punto;
}

int main(void) {
    uint16_t x[] = {
         2, 86, 77,  9,  1, 70, 95, 59,
        74, 37, 70, 34, 30, 11,  2, 15,
         3, 52, 11, 37, 15, 92, 39, 86,
        56, 42,  5, 68, 91, 22, 81, 10,
        79,  8, 24, 73, 82, 68, 22,  4,
        33, 97, 13, 46, 14, 10, 31, 96,
        28, 11, 48,  6, 80, 58, 50, 64,
        43, 73, 25, 67, 45, 57,  5, 61,
        74, 94, 99, 67, 64,  5, 94, 52,
        64, 81, 27, 45, 56, 25,  3, 63,
         6, 67, 97, 74,  9, 84, 86, 99,
        57, 68, 67, 67, 34, 84, 45, 65,
        50,  3, 33, 44, 16, 50, 72,  6,
        79, 26, 48, 65,  3,  2, 67, 80,
        45, 86, 61, 85, 82,  8, 26, 39,
        15, 14, 54, 98, 35, 65, 66, 96
    };
    uint16_t y[] = {
        87, 72, 48,  3, 79,  8, 29, 66,
        84, 90, 75, 88, 67, 29, 98, 86,
        56,  1, 29, 20, 95, 76,  9, 88,
        85, 24, 64, 46, 56, 21, 87, 28,
        38, 80, 77, 35, 33, 63, 13,  2,
        66, 86, 12, 71, 51, 20, 80, 86,
        54, 64, 79, 71, 54, 52, 85, 29,
         3, 22, 79, 15, 86, 83, 11, 87,
        31, 74, 31, 64, 68, 89, 21, 26,
        62, 50, 21, 33, 43, 50, 61, 27,
        31, 74, 79, 14, 76, 81, 67, 46,
        38, 15, 40, 85, 12, 66, 38, 68,
        73, 32,  8, 51, 40, 20, 53, 47,
        97, 98,  7, 22,  8,  5,  4,  6,
        24,  7, 53, 36, 43, 57, 26,  5,
        33, 29, 87, 83, 41, 50, 12, 98            
    };
    
    printf("Normal: %d\n", producto_punto(array_size(x), x, y));
    printf("OpenMP: %d\n", producto_punto_openmp(array_size(x), x, y));
    printf("SIMD:   %d\n", producto_punto_simd(array_size(x), x, y));
    return EXIT_SUCCESS;
}
