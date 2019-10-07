//#include <stdlib.h>
//#include <stdio.h>
#include <time.h>
#include <iostream>

//using namespace std;

int main(int argc, char *argv[]){
	int n = 4096; 
	int A[n][n];
	int B[n][n];
	int C[n][n];
	for (int i = 0; i < n; i++){
		for (int j =0; j < n; j++){
			A[i][j] = i;
			B[i][j] = i+j;
			C[i][j] = 0;
		}
	}
	//Start counting time 
	struct timespec start, stop;
	double t;
	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
	{ 
		perror( "clock gettime" );
	}

	//Write your code here 
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			for (int k = 0; k < n; k++){
				C[i][j] = C[i][j] + A[i][k]*B[k][j];
			}
		}
	}

	//End counting time
	if(clock_gettime(CLOCK_REALTIME, &stop)  == -1)
	{
		perror( "clock gettime" );
	}
	t = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec)/1e9;
	cout << t << endl;

	//printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);		
	//printf("C[100][100]=%f\n", C[100][100]);
	std::cout << "Number of FLOPS =" << 2*n*n*n << std::endl;
	std::cout << "Execution time (in secs) = " << t << std::endl;
	std::cout << "MFLOPs per sec = " << 1/t/1e6*2*n*n*n << std::endl;
	std::cout << C[100][100] << std::endl;

	return 0;
}