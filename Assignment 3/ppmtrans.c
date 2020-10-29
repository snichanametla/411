/*  
    Rotates an image by 0, 90, 180, and 270 degrees
    flips an image horizontally and vertically
    transposes an image
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"

#include "pnm.h"
#include <mem.h>
// rotate: calls the correct rotate function based on the given
//  rotation degrees. Outputs the rotated pnm file to 
       //standard output 
void rotate90(int i, int j, A2Methods_Array2 array, void *elem, void *cl);
void rotate180(int i, int j, A2Methods_Array2 array, void *elem, void *cl);
void rotate270(int i, int j, A2Methods_Array2 array, void *elem, void *cl);
void flip_horizontal(int i, int j, A2Methods_Array2 array, void *elem, void *cl);
void flip_vertical(int i, int j, A2Methods_Array2 array, void *elem, void *cl);
void transpose_image(int i, int j, A2Methods_Array2 array, void *elem, void *cl);



int main(int argc, char *argv[]) {
  int rotation = -1;
  int flip = -1;
  int transpose = -1;

  FILE* image_input = NULL;
  A2Methods_T methods = array2_methods_plain; // default to UArray2 methods
  assert(methods);
  A2Methods_mapfun *map = methods->map_default; // default to best map
  assert(map);

  
  
  //this was given from TA, old SET_METHODS was not working.
#define SET_METHODS(METHODS, MAP, WHAT)                                  \
  do                                                                     \
  {                                                                      \
    methods = (METHODS);                                                 \
    assert(methods);                                                     \
    map = methods->MAP;                                                  \
    if (!map)                                                            \
    {                                                                    \
      fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
      exit(1);                                                           \
    }                                                                    \
  } while (0)

  int i;
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-row-major")) {
      SET_METHODS(array2_methods_plain, map_row_major, "row-major");
    } else if (!strcmp(argv[i], "-col-major")) {
      SET_METHODS(array2_methods_plain, map_col_major, "column-major");
    } else if (!strcmp(argv[i], "-block-major")) {
      SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
    } else if (!strcmp(argv[i], "-rotate")) {
      assert(i + 1 < argc);
      char *endptr;
      rotation = strtol(argv[++i], &endptr, 10);
      assert(*endptr == '\0'); // parsed all correctly
      assert(rotation == 0   || rotation == 90
          || rotation == 180 || rotation == 270);
    } else if (!strcmp(argv[i], "-flip")){
      assert(i + 1 < argc);
      assert(!strcmp(argv[i+1], "horizontal") || !strcmp(argv[i+1], "vertical"));
      if(!strcmp(argv[++i], "horizontal")){
        flip = 1;
      } else if(!strcmp(argv[i], "vertical")){
        flip = 0;
      }
    } else if (!strcmp(argv[i], "-transpose")){
        transpose = 1;
    } else if (*argv[i] == '-') {
      fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
      exit(1);
    } else if (argc - i > 2) {
      fprintf(stderr, "Usage: %s [-rotate <angle>] "
              "[-{row,col,block}-major] [filename]\n", argv[0]);
      exit(1);
    } else {
      //opens file
      image_input = fopen(argv[i], "rb");
      break;
    }
    if(image_input == NULL){
      image_input = stdin;
    }
  }
  assert(image_input);

  Pnm_ppm input;
//correctly reads the file
  TRY
    input = Pnm_ppmread(image_input, methods);

  EXCEPT(Pnm_Badformat)
    fprintf(stderr, "Not valid file format\n");
    exit(1);
  
  END_TRY;
  
  

   A2Methods_Array2 outputImage = NULL;

  
  //we are checking this transformation (extra credit)
   if(rotation == 0){
      Pnm_ppmwrite(stdout,input); //writes out 
  }

  //we are checking this transformation
  else if(rotation == 90){
    outputImage = methods->new(input->height,input->width,sizeof(struct Pnm_rgb));
    map(outputImage,rotate90,input);
     
    Pnm_ppm output;
    NEW(output);

    int temp = input->width;
    output->width = input->height;
    output->height = temp;
    output->pixels = outputImage;
    output->denominator = input->denominator;
    output->methods = methods;
    Pnm_ppmwrite(stdout, output);
    Pnm_ppmfree(&input);
    Pnm_ppmfree(&output);
 
  }

  //we are checking this transformation
  else if(rotation == 180){
    outputImage = methods->new(input->width,input->height,sizeof(struct Pnm_rgb));
    map(outputImage,rotate180,input);
    
     
    Pnm_ppm output;
    NEW(output);

    
    output->width = input->width;
    output->height = input->height;
    output->pixels = outputImage;
    output->denominator = input->denominator;
    output->methods = methods;
    Pnm_ppmwrite(stdout, output);
    Pnm_ppmfree(&input);
    Pnm_ppmfree(&output);
  }

  //we are  checking this transformation
  else if(rotation == 270){
    outputImage = methods->new(input->height,input->width,sizeof(struct Pnm_rgb));
    map(outputImage,rotate270,input);
     
    Pnm_ppm output;
    NEW(output);

    int temp = input->width;
    output->width = input->height;
    output->height = temp;
    output->pixels = outputImage;
    output->denominator = input->denominator;
    output->methods = methods;
    Pnm_ppmwrite(stdout, output);
    Pnm_ppmfree(&input);
    Pnm_ppmfree(&output);
  } 

  else if(flip == 1){
    outputImage = methods->new(input->width,input->height,sizeof(struct Pnm_rgb));
    map(outputImage,flip_horizontal,input);
     
    Pnm_ppm output;
    NEW(output);

    
    output->width = input->width;
    output->height = input->height;
    output->pixels = outputImage;
    output->denominator = input->denominator;
    output->methods = methods;
    Pnm_ppmwrite(stdout, output);
    Pnm_ppmfree(&input);
    Pnm_ppmfree(&output);
  }

  else if(flip == 0){
    outputImage = methods->new(input->width,input->height,sizeof(struct Pnm_rgb));
    map(outputImage,flip_vertical,input);

    Pnm_ppm output;
    NEW(output);
    
    output->width = input->width;
    output->height = input->height;
    output->pixels = outputImage;
    output->denominator = input->denominator;
    output->methods = methods;
    Pnm_ppmwrite(stdout, output);
    Pnm_ppmfree(&input);
    Pnm_ppmfree(&output);
  }

  else if(transpose == 1){
    outputImage = methods->new(input->width,input->height,sizeof(struct Pnm_rgb));
    map(outputImage,transpose_image,input);
    
    Pnm_ppm output;
    NEW(output);

    output->width = input->width;
    output->height = input->height;
    output->pixels = outputImage;
    output->denominator = input->denominator;
    output->methods = methods;
    Pnm_ppmwrite(stdout, output);
    Pnm_ppmfree(&input);
    Pnm_ppmfree(&output);
  }
}


/*this function rotates the file by creating a new pnm file
and copying the pixels */
void rotate90(int i, int j, A2Methods_Array2 array, void *elem, void *cl){
  Pnm_ppm image = cl;
  
  int height = image->height;
  Pnm_rgb in = elem;
  Pnm_rgb out = image->methods->at(image->pixels,j,(height-i-1));
 
  *in = *out;
  (void)array;

}

/*this function rotates the file by creating a new pnm file
and copying the pixels */
void rotate180(int i, int j, A2Methods_Array2 array, void *elem, void *cl){
  Pnm_ppm image = cl;
  
  int height = image->height;
  int width = image->width;
  Pnm_rgb in = elem;
  Pnm_rgb out = image->methods->at(image->pixels,(width - i - 1),(height-j-1));
 
  *in = *out;

  (void)array;
  
   
}

void rotate270(int i, int j, A2Methods_Array2 array, void *elem, void *cl){
   Pnm_ppm image = cl;
  
  int width = image->width;
  Pnm_rgb in = elem;
  Pnm_rgb out = image->methods->at(image->pixels,(width - j - 1),i);
 
  *in = *out;

  (void)array;
}

void flip_horizontal(int i, int j, A2Methods_Array2 array, void *elem, void *cl){
  Pnm_ppm image = cl;
  
  int width = image->width;
  Pnm_rgb in = elem;
  Pnm_rgb out = image->methods->at(image->pixels,((width - 1) - i),j);
 
  *in = *out;
  (void)array;
   
}

void flip_vertical(int i, int j, A2Methods_Array2 array, void *elem, void *cl){
  Pnm_ppm image = cl;
  
  int height = image->height;
  Pnm_rgb in = elem;
  Pnm_rgb out = image->methods->at(image->pixels, i, ((height - 1) - j));
 
  *in = *out;
  (void)array;
   
}

void transpose_image(int i, int j, A2Methods_Array2 array, void *elem, void *cl){
  Pnm_ppm image = cl;
  
  int height = image->height;
  int width = image->width;
  Pnm_rgb in = elem;
  Pnm_rgb out = image->methods->at(image->pixels, ((width - 1) - i), ((height - 1) - j));
 
  *in = *out;
  (void)array;
   
}
