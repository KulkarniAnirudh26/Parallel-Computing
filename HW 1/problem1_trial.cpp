#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>

int main(void){
		int i, j, k;
		struct timespec start, stop; 
		double time;
		int n = 4096; // matrix size is n*n
		
		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}
		
		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;			
			}
		}
				
		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				for (int k = 0; k < n; k++){
					C[i][j] = C[i][j] + A[i][k]*B[k][j];
				}
			}
		}
		
		//*******************************//
		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
		float t = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

		//std::cout << "Number of FLOPS =" << 2*n*n*n << std::endl;
		std::cout << "Execution time (in secs) = " << t << std::endl;
		std::cout << "MFLOPs per sec = " << 1/t/1e6*2*n*n*n << std::endl;
		std::cout << C[100][100] << std::endl;
		

		// release memory
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);
		}
		free(A);
		free(B);
		free(C);
		return 0;
}
