#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string.h>

#define input_file "number.txt"


main(int argc, char *argv[])
{
		int npes, myrank;
		//Reading from txt
		int a[64];
		std::ifstream file;
		file.open(input_file);

		for (int i = 0; i < 64; i++){
			file >> a[i];
		}

		file.close();
		//std::cout << "Read" << a[2] << std::endl;
		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &npes);  // total number of processes
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
		
		MPI_Status status;
		//printf("From process %d out of %d, Hello World!\n",myrank, npes);
		int Start;
		int End;
		int sum = 0;
		int sum1 = 0;
		if (myrank == 0){
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			std::cout << Start << End << std::endl;
			for (int i = Start; i <= End; i++){
				sum1 = sum1 + a[i];
			}

			//Partial sum from other processors 
			MPI_Recv(&sum, 1, MPI_INT, 1, 201, MPI_COMM_WORLD, &status);
			sum1 = sum1 + sum;
			MPI_Recv(&sum, 1, MPI_INT, 2, 202, MPI_COMM_WORLD, &status);
			sum1 = sum1 + sum;
			MPI_Recv(&sum, 1, MPI_INT, 3, 203, MPI_COMM_WORLD, &status);
			sum1 = sum1 + sum;

			std::cout << "The final sum is " << sum1 << std::endl;

		}
		else if (myrank == 1){
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}
			MPI_Send(&sum,1,MPI_INT,0,201,MPI_COMM_WORLD);

		}
		else if (myrank == 2){
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}
			MPI_Send(&sum,1,MPI_INT,0,202,MPI_COMM_WORLD);

		}
		else{
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}
			MPI_Send(&sum,1,MPI_INT,0,203,MPI_COMM_WORLD);
			
		}
		

		MPI_Finalize();
}
