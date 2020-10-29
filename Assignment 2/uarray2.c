//Lauren Tinkham
//Pranitha Nichanametla 
//CSC 411
//uarray2.c file Program
 
#include "array.h"
#include "mem.h"
#include "uarray2.h"
#include <stdlib.h>

#define T UArray2_T 

//typedef struct T *T;
struct T{
    int row;
    int col;
    Array_T uarray;
};

/*creates a 2D using one a 1D array on the heap
  row and col must represent the number of rows and columns needed in 2D array
  size must represent the size of each element that is to be stored in the array
*/
T UArray2_new(int row, int col, int size){
    T uarray;
    NEW(uarray);
    uarray->row = row;
    uarray->col = col;
    uarray->uarray = Array_new(row*col, size);
    return uarray;
}
 
//  Returns the int total number of rows in the 2D array. 
int UArray2_rows(T uarray){
    return uarray->row;
}
 
//  Returns the int total number of columns in the array.
int UArray2_cols(T uarray){
    return uarray->col;
}
 
/*  calls apply on each element in row major order
    the function traverses through each row. Uses void *el and void *cl
    to store and access information from the elements.
*/
void UArray2_map_row_major(T uarray, 
    void apply(int i, int j, UArray2_T array2, void *elem, void *cl), void *cl){
    for(int r = 0; r < uarray->row; r++){
        for(int c = 0; c < uarray->col; c++){
            apply(c, r, uarray, UArray2_at(uarray, r, c), cl);
        }
    }
}
 
/*  calls apply on each element in column major order
    the function traverses through each column. Uses void *el and void *cl
    to store and access information from the elements.
*/
void UArray2_map_col_major(T uarray, void apply(int i, int j, UArray2_T array2, void *elem, void *cl), void *cl){
    for(int c = 0; c < uarray->col; c++){
        for(int r = 0; r < uarray->row; r++){
            apply(c, r, uarray, UArray2_at(uarray, r, c), cl);
        }
    }
}
 
/*  Returns a pointer to the element of index (row, col) in the array.
    There is no set/put function. To set the value at index(row,col), change set the returned
    pointer to desired value. The 1D index of an element stored in [row][col] of a 2D array
    is always equal to: (row * #of columns) + col.
*/
void *UArray2_at(T uarray, int row, int col){
    int i = (row * uarray->col) + col;
    return Array_get(uarray->uarray, i);
}
 
/*  Frees the 2D array from memory.
    First frees the array in struct, then frees the struct and remaining memory.
*/
void UArray2_free(T *uarray){
    Array_free(&((*uarray)->uarray));
    free(*uarray);
}
 
//  Returns the total number of elements in the array. (Is also # of rows * #of col)
int UArray2_length(T uarray){
    return Array_length(uarray->uarray);
}
 
//  Returns the  space in memory taken up by an element of the array. */
int UArray2_size(T uarray){
    return Array_size(uarray->uarray);
}
 
#undef T
