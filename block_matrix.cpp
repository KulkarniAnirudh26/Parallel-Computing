#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>

int main(int argc, char *argv[]){
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

		for (int g = 0; g < 10; g++){
				
			if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
			
			// Your code goes here //
			// Matrix C = Matrix A * Matrix B //	
			//*******************************//
			int m = atoi(argv[1]);
			//int m = 8;
			//std::cout << m ;
			int b = int(n/m); 
			for (int i =0; i < m; i++){
				for (int j=0; j < m; j++){
					for (int k =0; k < m; k++){
						//Above three loops were for creating the matrix blocks 
						//Now that we have the blocks, we should do matrix-mulitpication of the inner blocks 
						for (int p = i*b; p < (i+1)*b; p++) {//b = n/m
							for (int q = j*b; q < (j+1)*b; q++){
								for (int r = k*b; r < (k+1)*b; r++){
									C[p][q] = C[p][q] + A[p][r]*B[r][q];
								}
							}

						}
					}
				}
			}
			
			
			//*******************************//
			
			if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
			float t = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
			
			//printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);		
			//printf("C[100][100]=%f\n", C[100][100]);
			int n = 4096;
			//std::cout << "Number of FLOPS =" << 2*n*n*n << std::endl;
			std::cout << "Execution time (in secs) = " << t << std::endl;
			std::cout << "MFLOPs per sec = " << 1/t/1e6*2*n*n*n << std::endl;
			std::cout << C[100][100] << std::endl;

	}
		
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
