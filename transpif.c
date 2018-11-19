#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/*
Initialzies the input matrix with a random float between 0 and 10
*/
void initialize_input_matrix(float *input_matrix, int matrix_width, int matrix_height){
    int i, j;
    float r;

    srand(time(NULL));
    for(i = 0; i < matrix_height; i++){
        for (j = 0; j < matrix_width; j ++){
            r = ((float)rand() / RAND_MAX) * 10;
            input_matrix[i* matrix_width + j] = r;
        }
    }
}

/*
Prints the matrix
*/
void print_matrix(float * input_matrix, int matrix_width, int matrix_height){
    int i, j;

    srand(time(NULL));
    for(i = 0; i < matrix_height; i++){
        for(j = 0; j < matrix_width; j ++){
            printf("%f  ", input_matrix[i * matrix_width + j]);
        }
        printf("\n");
    }
}

/*
Transposes a rectangular matrix by transposing blocks at a time to increase locality.
*/
int transpose_matrix(float * input_matrix, float * output_matrix, int matrix_width, int matrix_height, int block_width){
    int i, j, r, c;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for(i = 0; i < matrix_height; i+= block_width){
        for (j = 0; j < matrix_width; j+= block_width){
            for(r = i; r < i + block_width && r < matrix_height; r++){
                for(c = j; c < j + block_width && c < matrix_width; c++){

                    //This if statement is redundent because my for loops take care of the case where you access "invalid"
                    // memory. The assignment specified if statments so I decided to put one in just in case
                    if (c >= matrix_width || r >= matrix_height){
                        printf("Error, accessing a value in a seperate row or column that should not be accessed yet");
                        return -1;
                    }

                    // printf("input matrix at %d is  %f \n",r * matrix_width + c, input_matrix[r * matrix_width + c]);
                    output_matrix[c * matrix_height + r] = input_matrix[r * matrix_width + c];
                    // printf("output matrix at %d is  %f  \n", c * matrix_height + r, output_matrix[c * matrix_height + r]);
                    
                }
            }
        }
    }

    gettimeofday(&end, NULL);
    
    unsigned long begin = 1000000 * start.tv_sec + start.tv_usec;
    unsigned long finish = 1000000 * end.tv_sec + end.tv_usec;

    printf("\n\nTime it took to complete transposition in micro seconds: %ld\n\n", finish - begin);
    return 1;
}


int main(int argc, char **argv)
{
    int matrix_width, block_width, matrix_height;
	if (argc != 4) {
		fprintf(stderr,
		"Usage: transp <matrix_width> <matrix_height> <block_width>\n");
		return -1;
	}

	matrix_width = atoi(argv[1]);
    matrix_height = atoi(argv[2]);
    block_width = atoi(argv[3]);

    float *input_matrix = malloc(matrix_width * matrix_height * sizeof(float));
    float *output_matrix = malloc(matrix_width * matrix_height * sizeof(float));

    initialize_input_matrix(input_matrix, matrix_width, matrix_height);
    //print_matrix(input_matrix, matrix_width, matrix_height);

    transpose_matrix(input_matrix, output_matrix, matrix_width, matrix_height, block_width);

    printf("\n\n");
    //print_matrix(output_matrix, matrix_height, matrix_width); // switched height and width from first one because it is the transposed matrix

    free(input_matrix);
    free(output_matrix);

	return 1;
}