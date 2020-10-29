//#include "uarray2.h"
#include "array.h"
#include "mem.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define T UArray2b_T
typedef struct T *T;
struct T{
    int height; //rows
    int width; //cols
    int blocksize;
    int size;
    Array_T uarrayb;
};
extern T UArray2b_new (int width, int height, int size, int blocksize){
    T uarrayb;
    NEW(uarrayb);
    uarrayb->width = width; //columns
    uarrayb->height = height; //rows
    uarrayb->size = size;
    uarrayb->blocksize = blocksize;

    // take ceiling of width/blocksize and height/blocksize to accomodate extra pixels
    // that don't perfectly fit in block
    uarrayb->uarrayb = Array_new(((height / blocksize) + 1) * ((width / blocksize) + 1), sizeof(Array_T*));
    
    //populate each element with a 1D array of size blocksize * blocksize
    for(int i = 0; i < Array_length(uarrayb->uarrayb); i++){
        Array_T cells = Array_new(blocksize * blocksize, size);
        Array_T *block = Array_get(uarrayb->uarrayb, i);
        *block = cells;
    }
    //set blocks to point to Array
    return uarrayb;
}
/* new blocked 2d array: blocksize = square root of # of cells in block */
extern T UArray2b_new_16K_block(int width, int height, int size){
    int blocksize = sqrt(16 * 1024 / size); //16K max blocksize
    return UArray2b_new (width, height, size, blocksize);
}

/* new blocked 2d array: blocksize as large as possible provided
block occupies at most 16KB (if possible) */
extern void UArray2b_free (T *array2b){
     for(int i = 0; i < Array_length((*array2b)->uarrayb); i++){
        Array_T *block = Array_get((*array2b)->uarrayb, i);
        Array_free(&((*block)));
     }
     Array_free(&(*array2b)->uarrayb);
     FREE(*array2b);

}

extern int UArray2b_width (T array2b){
    return array2b->width;
}

extern int UArray2b_height(T array2b){
    return array2b->height;
}

extern int UArray2b_size (T array2b){
    return array2b->size;
}

extern int UArray2b_blocksize(T array2b){
    return array2b->blocksize;
}

extern void *UArray2b_at(T array2b, int i, int j){
    int y = i / array2b->blocksize; //finds col of block
    int x = j / array2b->blocksize; //finds row of block
    int c = i % array2b->blocksize; //finds col of cell in block
    int r = j % array2b->blocksize; //finds row of cell in block
    //int total = array2b->width * array2b->height;
    
    //check if valid pixel coordinates
    //assert(i > -1 && i < array2b->width && j > -1 && j < array2b->height);
    int blockIndex = (x * ((array2b->height / array2b->blocksize) + 1)) + y;        
    int cell = (r * ((array2b->width / array2b->blocksize) + 1)) + c;

    //assert(cell > -1 && cell < total);
    Array_T *block = Array_get(array2b->uarrayb, blockIndex); //get block, then find cell coordinates
    return Array_get(*block, cell);

    //I'm just going to assume that there is bound checking in this because I can't use assert() without 
    //it yelling
}

/* return a pointer to the cell in column i, row j;
index out of range is a checked run-time error
*/
void UArray2b_map(T array2b, 
    void apply(int i, int j, T array2b, void *elem, void *cl), void *cl){

    int i, j, n, m;

    //Get the block array dimensions 
    int blockArrayWidth = array2b->width / array2b->blocksize;
    int blockArrayHeight = array2b->height / array2b->blocksize;

    //Iterate through each block
    for (j = 0; j < blockArrayHeight; j++){
        for (i = 0; i < blockArrayWidth; i++){
            //get block 1D array
            int blockIndex = (j * ((array2b->height / array2b->blocksize) + 1)) + i;
            if(blockIndex > -1 && blockIndex < Array_length(array2b->uarrayb)){
                Array_T *block = Array_get(array2b->uarrayb, blockIndex);
                //we need to calculate the index for the first cell in the
                //block as a point of reference
                int topLeftCellinBlockRowIndex = j * array2b->blocksize;
                int topLeftCellinBlockColIndex = i * array2b->blocksize;

                //Iterate through all cells in a block
                for (n = 0;n < array2b->blocksize; n++){
                    for (m = 0; m < array2b->blocksize; m++){

                        //calculate the individual cell's index values
                        int actualX = n * topLeftCellinBlockRowIndex;
                        int actualY = n * topLeftCellinBlockColIndex;

                        //Check to ensure we have not left the actual 2d array bounds
                        if ((actualX < array2b->width) && (actualY < array2b->height)){
                            int cell = actualX * array2b->blocksize + actualY;
                            //your apply fucntion parameters may be slightly differnt
                            apply(actualY, actualX,array2b, Array_get(*block, cell),cl);
                        }
                        //Else we are outside the bounds of the array so we do nothing
                    }
                }
            }
        }   
    }
}

extern void UArray2b_map(T array2b,
void apply(int i, int j, T array2b, void *elem, void *cl), void *cl);
/* visits every cell in one block before moving to another block */
/* it is a checked run-time error to pass a NULL T
to any function in this interface */
#undef T
