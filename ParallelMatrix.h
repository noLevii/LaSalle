//
// ParallelMatrix.h
// Practica4
//
//

#pragma once

#ifndef ParallelMatrix_h
#define ParallelMatrix_h

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Prototipos de funciones
int get_number_of_cpus();
int check_sum_all(long double *, long double *, int, int);
int omp_parallel_matrix_sum(long double **, long double *, long double *, int, int);
int matrix_sum(long double **, long double *, long double *, int, int);

int matrix_multi (
    long double *result, long double *A, long double *B, int mat_size, int mat_size2
);

int matrix_multi_parallel (
    long double *result, long double *A, long double *B, int mat_size, int mat_size2
);

long double matrix_max(long double *A, int size_mat, int size_mat2);
long double matrix_max_multi(long double *A, int size_mat, int size_mat2);

#endif
