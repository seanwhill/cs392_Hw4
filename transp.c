#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/*
Initializes the input matrix to be filled with random numbers between 0 and 10

*/
void initialize_input_matrix(float *input_matrix, int matrix_width){
    int i, j;
    float r;

    srand(time(NULL));
    for(i = 0; i < matrix_width; i++){
        for (j = 0; j < matrix_width; j ++){
            r = ((float)rand() / RAND_MAX) * 10;
            input_matrix[i* matrix_width + j] = r;
        }
    }
}

/*
Prints the matrix
*/
void print_matrix(float * input_matrix, int matrix_width){
    int i, j;

    srand(time(NULL));
    for(i = 0; i < matrix_width; i++){
        for(j = 0; j < matrix_width; j ++){
            printf("%f  ", input_matrix[i * matrix_width + j]);
        }
        printf("\n");
    }
}

/*
Transposes the matrix using blocks to optimize spatial locality to decrease cache misses.
*/
void transpose_matrix(float * input_matrix, float * output_matrix, int matrix_width, int block_width){
    int i, j, r, c;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for(i = 0; i < matrix_width; i += block_width){
        for (j = 0; j < matrix_width; j+= block_width){
            for(r = i; r < i + block_width; r++){
                for(c = j; c < j + block_width; c++){
                    output_matrix[c * matrix_width + r] = input_matrix[r * matrix_width + c];
                }
            }
        }
    }

    gettimeofday(&end, NULL);

    unsigned long begin = 1000000 * start.tv_sec + start.tv_usec;
    unsigned long finish = 1000000 * end.tv_sec + end.tv_usec;

    printf("Time it took to complete transposition in micro seconds: %ld", finish - begin);
}

/*
Non efficent matrix transposition used for testing in valgrind.
void not_efficent_transp(float * input_matrix, float * output_matrix, int matrix_width){
    int i, j;
    for( int i = 0; i < matrix_width; i++ ){
        for( int j = 0; j < matrix_width; j++ ){
            output_matrix[j*matrix_width + i] = input_matrix[i*matrix_width +j];
        }
    }
}
*/

int main(int argc, char **argv)
{
    int matrix_width, block_width;
	if (argc != 3) {
		fprintf(stderr,
		"Usage: transp <matrix_width> <block_width>\n");
		return -1;
	}

	matrix_width = atoi(argv[1]);
    block_width = atoi(argv[2]);

    float *input_matrix = malloc(matrix_width * matrix_width * sizeof(float)); //allocates a matrix of width matrix_width
    float *output_matrix = malloc(matrix_width * matrix_width * sizeof(float)); //allocates a matrix of width matrix_width

    initialize_input_matrix(input_matrix, matrix_width);

    //print_matrix(input_matrix, matrix_width);
    transpose_matrix(input_matrix, output_matrix, matrix_width, block_width);

    printf("\n\n");
    //print_matrix(output_matrix, matrix_width);

    free(input_matrix);
    free(output_matrix);

	return 1;
}