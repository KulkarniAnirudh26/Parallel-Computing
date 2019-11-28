#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <fstream>


#define input_file "number.txt"


int main(int argc, char *argv[])
{
		int npes, myrank;

		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &npes);  // total number of processes
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
		
		MPI_Status status;

		int a[64];
		int b[15];
		int sum=0;
		int sum1[4];
		if (myrank == 0){
			std::ifstream file;
			file.open(input_file);
			for (int i = 0; i < 64; i++){
				file >> a[i];
			}
		}
		MPI_Scatter(&a, 16, MPI_INT, &b, 16, MPI_INT, 0, MPI_COMM_WORLD);

		if (myrank == 0){
			for (int i = 0; i <= 15; i++){
				sum = sum + b[i];
			}
			
		}
		else if (myrank == 1){
			for (int i = 0; i <= 15; i++){
				sum = sum + b[i];
			}	

		}
		else if (myrank == 2){
			for (int i = 0; i <= 15; i++){
				sum = sum + b[i];
			}	

		}
		else{
			for (int i = 0; i <= 15; i++){
				sum = sum + b[i];
			}
			
		}
		MPI_Gather(&sum, 1, MPI_INT, sum1, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if (myrank == 0){
			int final = 0;
			for (int i = 0; i < 4; i++){
				final = final + sum1[i];
			}
			std::cout << "The final sum is " << final << std::endl;
		}


		MPI_Finalize();
		return 0;
}