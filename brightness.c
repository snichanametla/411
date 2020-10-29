#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pnmrdr.h"
#include "assert.h"

int main(int argc, char *argv[])
{
        //make sure at most 1 argument is given
        assert(argc <=2);
        FILE *input;
        //open file with name
        if (argc ==2){
                input = fopen(argv[1], "rb");
                if (!input){
                        perror(argv[1]);
                        exit(1);
                }
        }else{
                input = stdin;
        }

        //create the pnmrdr data
        Pnmrdr_T r = Pnmrdr_new(input);
        Pnmrdr_mapdata data = Pnmrdr_data(r);
        int total_pixels = data.width * data.height;

        //calculate the average of all pixels
        float sum = 0;
        float numerator = 0;
        for(int i = 0; i < total_pixels; i++){
                numerator = Pnmrdr_get(r);
                sum  = sum + (numerator/data.denominator);
        }

        sum = sum/total_pixels;

        printf("%.3f\n", sum);

        Pnmrdr_free(&r);
        fclose(input);

        return 0;
}
