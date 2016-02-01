/*
 +­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 
 | Nombre:    Marco Antonio Chávez Fuentes
 | Fecha:     14 de mayo, 2010.
 +­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 
 */

#include <stdlib.h>
#include <stdio.h>
#include <bmRGB.h>

#define CICLOS 10

#ifdef VERSION_OPENMP
	#include <omp.h>
#endif

#ifdef VERSION_SIMD
	#include <emmintrin.h>
#endif

bmRGB_status_t bmRGB_combine(int alpha, bmRGB_t* obj1, bmRGB_t* obj2, const char* name);

int main (int args, char *arg[]){
	
	bmRGB_t fore_image;
	bmRGB_t back_image;

	bmRGB_status_t status;
	status = bmRGB_create(arg[2], &fore_image);
	if(status != OK){
		printf("Error al procesar la primera imagen: (%d)\n",status);
	}

	status = bmRGB_create(arg[3], &back_image);
	if(status != OK){
		printf("Error al procesar la segunda imagen: (%d)\n",status);
	}

	int intensidad_alfa;
	sscanf(arg[1], "%d", &intensidad_alfa);

	#ifdef VERSION_OPENMP
		#ifdef VERSION_SIMD
			printf("Version SIMD OpenMP\n");
		#else
			printf("Version OpenMP\n");
		#endif
	#else	
		#ifdef VERSION_SIMD
			printf("Version SIMD\n");
		#else		
			printf("Version Secuencial\n");
		#endif
	#endif			
	
	for(int i=0; i<CICLOS; i++){
		status = bmRGB_combine(intensidad_alfa, &fore_image, &back_image, arg[4]);
	}
	
	bmRGB_dispose(&fore_image);
	bmRGB_dispose(&back_image);

	if(status != OK){
		printf("Error al procesar la imagen de salida: (%d)\n",status);
	}

	return EXIT_SUCCESS;
}

bmRGB_status_t bmRGB_combine(int alpha, bmRGB_t* fore_obj, bmRGB_t* back_obj, const char* name){
	
	if(fore_obj -> width != back_obj -> width || 
	   fore_obj -> height != back_obj -> height){	
		return BAD_BMP_FILE_ERROR;
	}
	
	bmRGB_t info;	
	info.contents = malloc(fore_obj -> size);
	if(info.contents == NULL){
		return OUT_OF_MEMORY_ERROR;	
	}

	info.num_pixels = fore_obj -> num_pixels;
	info.pixels = info.contents + PIXEL_OFFSET;
	info.size = fore_obj -> size;

	int info_bmRGB;
	#ifdef VERSION_OPENMP
		#pragma omp parallel for private(info_bmRGB)
	#endif
	for(int i=0; i< PIXEL_OFFSET; i++){
		info_bmRGB = fore_obj -> contents[i];
		info.contents[i] = info_bmRGB;
	}
	
	
	#ifdef VERSION_SIMD
	__m128i result;
		#ifdef VERSION_OPENMP
			#pragma omp parallel for private(result)
		#endif
	for(int i=0; i< info.num_pixels; i+=8){

		__m128i fore, back;
		//Paso 1
		fore = _mm_loadl_epi64((__m128i*)(fore_obj -> pixels +i));
		
		//Paso 2
		__m128i p = _mm_set1_epi8(0);
		__m128i fore_p = _mm_unpacklo_epi8(fore, p);
		
		//Paso 3
		__m128i alpha_p = _mm_set1_epi16(alpha);
		__m128i fore_alpha = _mm_mullo_epi16(fore_p, alpha_p);
		
		//Paso 4
		__m128i sum1 = _mm_srli_epi16(fore_alpha, 8);
		
		//Paso 5
		back = _mm_loadl_epi64((__m128i*)(back_obj -> pixels +i));
		
		back = _mm_unpacklo_epi8(back, p);
		__m128i constant2 = _mm_set1_epi16(255-alpha);
		__m128i back_alpha = _mm_mullo_epi16(back, constant2);
		__m128i sum2 = _mm_srli_epi16(back_alpha, 8);
		
		//Paso 6
		result =  _mm_add_epi16(sum1, sum2);
		
		//Paso 7
		result = _mm_packus_epi16(result, p);
		
		//Paso 8
		_mm_storel_epi64((__m128i*)(info.pixels + i), result);
	}

	#else
		
		int resultado_bmRGB;
		#ifdef VERSION_OPENMP
			#pragma omp parallel for private(resultado_bmRGB)
		#endif
			for(int i = 0; i < info.num_pixels; i++){
				int fore = fore_obj -> pixels[i];
				int back = back_obj -> pixels[i];
				resultado_bmRGB = (fore*alpha)/256 + (back * (255 - alpha)) / 256;
				info.pixels[i] = resultado_bmRGB;
			}
	#endif

	FILE* file = fopen(name, "wb");
	if(file == NULL){
		return FILE_ERROR;	
	}
	
	size_t bytes_written = fwrite( info.contents, sizeof(uint8_t), info.size, file);
	fclose(file);

	if(bytes_written != info.size){
		return FILE_ERROR;	
	}
	
	bmRGB_dispose(&info);

	return OK;

}
