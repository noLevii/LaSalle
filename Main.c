//
// Main.c
// Practica4
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "ParallelMatrix.h"

double timer_get_current_time(void) {
	struct timeval time = { 0 };
	return !gettimeofday (&time, NULL) ?
		(double) time.tv_sec + (double) time.tv_usec * .000001 : 0;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	
	//Variables a utilizar
		//Variables de la matriz
	long double* A = NULL;
	long double* B = NULL;
	long double* C = NULL;
	long double count = 1;
	int size_mat = 10, size_mat2 = 20;
	int row, col;
	long double tiempos[25][3][3] = {};
	int nivel = 0, colum = 0, r = 0;
		//Variable de los hilos
	int num_threads;
		//Alojamiento de las matrices
	A = (long double*)malloc(size_mat * size_mat2 * sizeof(long double));
	B = (long double*)malloc(size_mat * size_mat2 * sizeof(long double));
	C = (long double*)malloc(size_mat * size_mat2 * sizeof(long double));
	if(!A || !B || !C) {
		printf("No se pudo alojar las matrices...\n");
		exit(EXIT_FAILURE);
		}
	
	/*Instrucciones para realizar la simulacion con 1, 2, 3 y 4 hilos disponibles para 4 nucleos.*/
	
	num_threads = get_number_of_cpus();
	printf("Nucleos disponibles en la computadora: %d\n",num_threads);
	printf("Cantidad de hilos que se pidieron (max posibles): %d\n\n",num_threads);
	for(int num = 0; num < 6; num++)
	{
		printf("---------Matriz usada %d ------------",num);
		switch(num)
		{
			case 0:
				size_mat = 10;
				size_mat2 = 20;
				break;
			case 1:
                                size_mat = 50;
                                size_mat2 = 50;
                                break;
			case 2:
                                size_mat = 200;
                                size_mat2 = 100;
                                break;
			case 3:
                                size_mat = 1000;
                                size_mat2 = 1000;
                                break;
			case 4:
                                size_mat = 1200;
                                size_mat2 = 1200;
                                break;
			case 5:
                                size_mat = 1920;
                                size_mat2 = 1080;
                                break;
		}
		A = NULL;
		B = NULL;
		C = NULL;
		free(A);
		free(B);
		free(C);
		A = (long double*)malloc(size_mat * size_mat2 * sizeof(long double));
                B = (long double*)malloc(size_mat * size_mat2 * sizeof(long double));
                C = (long double*)malloc(size_mat * size_mat2 * sizeof(long double));
		if(!A || !B || !C) {
                	printf("No se pudo alojar las matrices...\n");
                	exit(EXIT_FAILURE);
                }
		
		for(row = 0; row < size_mat2; row++)
                {
                	for(col = 0; col < size_mat; col++)
                        {
                        	*(A + row * size_mat + col) = rand() % 256;
                                *(B + row * size_mat + col) = rand() % 256;
                                count++;
                        }
                 }
                 count = 1;

		
		for(int num2 = 1; num2 < num_threads + 1; num2++)
		{
			omp_set_num_threads(num2);
			printf("\nHilo usado %d %d %d\n", num2, size_mat,size_mat2);
        
        		//Funcion para sumar las matrices de forma no paralela y toma de tiempo.
        		double start = timer_get_current_time ();
        		//matrix_multi (C, A, B, size_mat,size_mat2);
        		//double matrix_multi_single_time = timer_get_current_time () - start;
			
			matrix_sum(&C,A,B,size_mat,size_mat2);
        		double matrix_multi_single_time = timer_get_current_time () - start;
			check_sum_all(&count,C,size_mat,size_mat2);
			printf("La suma de todos los componentes de A + B en no paralelo es: %Lf\n", count);
        		printf("process time: %f secs\n", matrix_multi_single_time);

        		memset(C, 0, size_mat2 * size_mat * sizeof(long double));

        		//Funcion para sumar las matrices de forma paralela y toma de tiempo.
        		start = timer_get_current_time ();
        		matrix_multi_parallel (C, A, B, size_mat,size_mat2);
        
        		omp_parallel_matrix_sum(&C,A,B,size_mat,size_mat2);
			double matrix_multi_multi_time = timer_get_current_time () - start;
			check_sum_all(&count,C,size_mat,size_mat2);
        		printf("La suma de todos los componentes de A + B en paralelo es: %Lf\n", count);
        		printf("process time: %f secs\n\n", matrix_multi_multi_time);

        		//Funcion para obtener el valor maximo de forma no paralela y toma de tiempo.
        		start = timer_get_current_time ();
        		long double max = matrix_max (A, size_mat,size_mat2);
        		double matrix_max_single_time = timer_get_current_time () - start;
        		printf("El maximo de A en no paralelo es: %Lf\n", max);
        		printf ("process time: %f secs\n", matrix_max_single_time);

        		//Funcion para obtener el valor maximo de forma paralela y toma de tiempo.
        		long double max_multi = matrix_max_multi (A, size_mat,size_mat2);
        		double matrix_max_multi_time = timer_get_current_time () - start;
        		printf("El maximo de A en paralelo es: %Lf\n", max_multi);
        		printf ("process time: %f secs\n", matrix_max_multi_time);

        		//Impresion de tabla de tiempos de TODA la simulacion.
			tiempos[nivel][r][colum] = matrix_multi_single_time;
			colum++;
			tiempos[nivel][r][colum] = matrix_multi_multi_time;
			r++;
			colum = 0;
			tiempos[nivel][r][colum] = matrix_max_single_time;
			colum++;
			tiempos[nivel][r][colum] = matrix_max_multi_time;
			r = 0;
			colum = 0;
			nivel++;

		}
		for(row = 0; row < size_mat2; row++)
                {
                        for(col = 0; col < size_mat; col++)
                        {
                                *(A + row * size_mat + col) = 0;
                                *(B + row * size_mat + col) = 0;
                        }
                 }
	}
	//Impresion de tabla de tiempos de TODA la simulacion.
	for(int niv = 0; niv < 24; niv++)
	{
		switch(niv)
		{
			case 0:
				printf("\n%s\nProceso|\tParalelo|\tNo Paralelo|\n", "Matriz 10x20");
				break;
			case 4:
				printf("\n%s\nProceso|\tParalelo|\tNo Paralelo|\n", "Matriz 50x50");
				break;
			case 8:
				printf("\n%s\nProceso|\tParalelo|\tNo Paralelo|\n", "Matriz 200x100");
				break;
			case 12:
				printf("\n%s\nProceso|\tParalelo|\tNo Paralelo|\n", "Matriz 1000,1000");
				break;
			case 16:
				printf("\n%s\nProceso|\tParalelo|\tNo Paralelo|\n", "Matriz 1200x1200");
				break;
			case 20:
				printf("\n%s\nProceso|\tParalelo|\tNo Paralelo|\n", "Matriz 1920x1080");
				break;
		}
		for(int ro = 0; ro < 2; ro++)
		{
			switch(ro)
			{
				case 0:
					printf("Suma\t|");
					break;
				case 1:
					printf("Max\t|");
					break;
			}
			for(int co = 0; co < 2; co++)
			{
				switch(co)
				{
					case 0:
						printf("%Lf secs\t|", tiempos[niv][ro][co]);
						break;
					case 1:
						printf("%Lf secs|\n", tiempos[niv][ro][co]);
						break;
				}
			}
		}
	}
	return EXIT_SUCCESS;
}
