//Lauren Tinkham
//Pranitha Nichanametla 
//CSC 411
//bit2.c file Program

#include <stdlib.h>
#include "array.h"
#include "mem.h"
#include "bit2.h"
#include "bit.h"
#include "assert.h"
 
#define T Bit2_T
 
//typedef struct T *T;
struct T{
    int row;
    int col;
    Bit_T barray;
};
 
/*creates a 2D using one a 1D array on the heap
  row and col must represent the number of rows and columns needed in 2D array
  size must represent the size of each element that is to be stored in the array
*/
T Bit2_new(int row, int col){
    T barray;
    NEW(barray);
    barray->row = row;
    barray->col = col;
    barray->barray = Bit_new(row*col);
    return barray;
}
 
//  Returns the int total number of rows in the 2D array of bits. 
int Bit2_rows(T barray){
    return barray->row;
}
 
//  Returns the int total number of columns in the array.
int Bit2_cols(T barray){
    return barray->col;
}

/*  calls apply on each element in row major order
    the function traverses through each row. Uses void *el and void *cl
    to access information from the elements. Uses Bit2_put to store information 
    into 2D bit array.
*/
void Bit2_map_row_major(T barray, void apply(T barray, int el, int row, int col, void *cl), void *cl){
    for(int r = 0; r < barray->row; r++){
        for(int c = 0; c < barray->col; c++){
            int temp = Bit2_get(barray, r, c);
            apply(barray, temp, r, c, cl);
            Bit2_put(barray, r, c, temp);
        }
    }
}

/*  calls apply on each element in column major order
    the function traverses through each column. Uses void *el and void *cl
    to access information from the elements. Uses Bit2_put to store information 
    into 2D bit array.
*/
void Bit2_map_col_major(T barray, void apply(T barray, int el, int row, int col, void *cl), void *cl){
    for(int c = 0; c < barray->col; c++){
        for(int r = 0; r < barray->row; r++){
            int temp = Bit2_get(barray, r, c);
            apply(barray, temp, r, c, cl);
            Bit2_put(barray, r, c, temp);
        }
    }
}

/*  Returns a copy of the value at [row][col] previous to the change and sets the bit at [row][col]
    to value. The value is checked to make sure it is a bit. Bounds are checked to make
    sure index does not go out of bounds. The 1D index of an element stored in [row][col] of a 2D array
    is always equal to: (row * #of columns) + col.
*/
int Bit2_put(T barray, int row, int col, int value){
    assert(barray);
    assert(value== 0 || value == 1);
    int i = (row * barray->col) + col;

    assert(0 <= i && i < Bit2_length(barray));
    return Bit_put(barray -> barray, i, value);
}

/*  Returns a copy of the bit's value at [row][col]. Bounds are checked to make
    sure index does not go out of bounds. The 1D index of an element stored in [row][col] of a 2D array
    is always equal to: (row * #of columns) + col.
*/
int Bit2_get(T barray, int x, int y){
   int i = (x * barray->col) + y;
   assert(0 <= i && i < Bit2_length(barray));
   return Bit_get(barray -> barray, i);
   
}
 
/*  Frees the 2D array from memory.
    First frees the array in struct, then frees the struct and remaining memory.
*/
void Bit2_free(T *barray){
    Bit_free(&((*barray)->barray));
    free(*barray);
}
 
// Returns the total number of elements in the array. (Is also # of rows * #of col)it2_length: returns the total number of elements in the array.
int Bit2_length(T barray){
    return Bit_length(barray -> barray); 
    
}
 
#undef T 
