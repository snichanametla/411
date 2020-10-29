//Lauren Tinkham
//Pranitha Nichanametla 
//CSC 411
//Sudoku Program
#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <except.h>
#include <assert.h>

#include "array.h"
#include "uarray2.h"
const int sudoku_size = 9;
FILE *input_file(int argc, char *argv[]);
void store_pixels(UArray2_T puzzle, Pnmrdr_T x, int row, int col);
void duplicate_array(UArray2_T puzzle, void *el, int row, int col, void *cl);
void check_doubles(Array_T puzzle);
 
 
int main(int argc, char *argv[]){
    // UArray2_T puzzle;
    //make sure input is either in command line or standard input
   
    assert(argc <= 2);
    FILE *input;
    input = input_file(argc, argv);
 
    Pnmrdr_T x = Pnmrdr_new(input);
    Pnmrdr_mapdata data = Pnmrdr_data(x);
 
    //make sure data in map is a sudoku puzzle
    if(data.width != 9 || data.height != 9 || data.denominator != 9){
        exit(1);
    }
 
    int row = data.height;
    int col = data.width;
    UArray2_T puzzle = UArray2_new(row, col, sizeof(int));
    store_pixels(puzzle, x, row, col);
    fclose(input);
    
    Array_T line = Array_new(9, sizeof(int));
    
    //rows
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){

            int *temp = UArray2_at(puzzle, r, c);
            int *temp2 =Array_get(line, c);
            
            *temp2 = *temp; 
        }
        check_doubles(line);//exit(1) if double is found; each row
    }
    //cols 
    for(int c = 0; c < 9; c++){
        for(int r = 0; r < 9; r++){
                int *temp_1 =UArray2_at(puzzle, c, r);
                int *temp_2 = Array_get(line, r);


                *temp_2 = *temp_1;
            }
            check_doubles(line); //exit(1) if double is found; each col
    }

    //submap
    
    for(int rindex = 0; rindex < 9; rindex += 3){ 
        for(int cindex = 0; cindex < 9; cindex += 3){ 
            int array_idx = 0;
            for(int i = rindex; i < rindex + 3; i++){ 
                for(int j = cindex; j < cindex + 3; j++){
                    int *temp_1 = UArray2_at(puzzle, i, j);
                    int *temp_2 = Array_get(line, array_idx);
                    *temp_2 = *temp_1;
                    array_idx++;
                }
            }
            check_doubles(line); //exit(1) if double is found; each submap
        }
    }

    Pnmrdr_free(&x);
    Array_free(&line);
    UArray2_free(&puzzle); // <-- There is an issue with invalid pointers with this free 
    exit(0);
}
 
 
FILE *input_file(int argc, char *argv[]){
    //make sure at most 1 argument is given
    assert(argc <= 2);
    FILE *input;
    //open file with name
    if (argc == 2){
        input = fopen(argv[1], "rb");
        if (!input){
            exit(1);
        }
    }else{
        input = stdin;
    }
    return input;
}
 
 
void store_pixels(UArray2_T puzzle, Pnmrdr_T x, int row, int col){
    int numerator = 0;
    for(int r = 0; r < row; r++){
        for(int c = 0; c < col; c++){
            numerator = Pnmrdr_get(x);
            if(!numerator || (numerator > 9 || numerator < 1)){
                exit(1);
            }
            int *p = UArray2_at(puzzle, r, c);
            *p = numerator;
        }
    }
}
 
void check_doubles(Array_T puzzle){
    int count = Array_length(puzzle);
    for (int row = 0; row < count; row++) { 
        for (int col = 0; col < count; col++) {
            int *index = Array_get(puzzle, col);
            int *current = Array_get(puzzle, row);
            if ((*index == *current) && (row!= col)){       
                exit(1);
            } 
        }
    }
}
