//
// ParallelMatrix.c
// Practica4
//
//

#include "ParallelMatrix.h"

//Definicion de funciones
int get_number_of_cpus() {
    int num_threads;
    num_threads = omp_get_max_threads();
    return num_threads;
}

int check_sum_all(long double *counter, long double *M, int size_mat, int size_mat2) {
    int element, total_elem;

    if(!counter || !M) {
        printf("Error in input pointers...\n");
        return EXIT_FAILURE;
    }

    total_elem = size_mat * size_mat2;
    *counter = 0;
    for(element = 0; element < total_elem; element++) {
        *counter = *counter + *(M + element);
    }

    return EXIT_SUCCESS;
}

int omp_parallel_matrix_sum(long double **result, long double *A, long double *B, int size_mat, int size_mat2) {
    int element, total_elem;
	
	if(!*result || !A || !B) {
		printf("Error in a matrix pointer...\n");
		return EXIT_FAILURE;
	}
	
	total_elem = size_mat * size_mat2;
	
	#pragma omp parallel for
	for(element = 0; element < total_elem; element++) {
		*(*result + element) = *(A + element) + *(B + element);
	}
	
	return EXIT_SUCCESS;
}

int matrix_sum(long double **result, long double *A, long double *B, int size_mat, int size_mat2) {
    int element, total_elem;
	
	if(!*result || !A || !B) {
		printf("Error in a matrix pointer...\n");
		return EXIT_FAILURE;
	}

    total_elem = size_mat * size_mat2;
    for(element = 0; element < total_elem; element++) {
        *(*result + element) = *(A + element) + *(B + element);
    }

    return EXIT_SUCCESS;
}

int matrix_multi (
    long double *result, long double *A, long double *B, int mat_size, int mat_size2
) {
    if(!*result || !A || !B) {
		printf("Error in a matrix pointer...\n");
		return EXIT_FAILURE;
	}

    int c = 0, d = 0, k = 0;
	long double *res = result;

	for (c = 0; c < mat_size2; c++) {
		for (d = 0; d < mat_size; d++) {
			for (k = 0; k < mat_size; k++) {
				*(res + c * mat_size + d) += *(A + c * mat_size + k) * *(B + k * mat_size + d);
			}
		}
	}

    return EXIT_SUCCESS;
}

int matrix_multi_parallel (
    long double *result, long double *A, long double *B, int mat_size, int mat_size2
) {
    if(!result || !A || !B) {
		printf("Error in a matrix pointer..\n");
		return EXIT_FAILURE;
	}

	long double *res = result;

	#pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < mat_size2; i++) { 
            for (int j = 0; j < mat_size; j++) {
                double dot  = 0;
                for (int k = 0; k < mat_size; k++) {
                    dot += A[i * mat_size + k] * B[k * mat_size + j];
                }

                res[i * mat_size + j] = dot;
            }
        }
    }

	return EXIT_SUCCESS;
}

long double matrix_max(long double *A, int size_mat, int size_mat2) {
    int element, total_elem;
    long double max = 0;

    total_elem = size_mat * size_mat2;
    for(element = 0; element < total_elem; element++) {
        if(*(A + element) > max) {
            max = *(A + element);
        }
    }

    return max;
}

long double matrix_max_multi(long double *A, int size_mat, int size_mat2) {
    long double max = 0;

    #pragma omp parallel
    {
        int element, total_elem;
        
        total_elem = size_mat * size_mat2;
        #pragma omp parallel for shared(max)
        for(element = 0; element < total_elem; element++) {
            if(*(A + element) > max) {
                max = *(A + element);
            }
        }
    }

    return max;
}
