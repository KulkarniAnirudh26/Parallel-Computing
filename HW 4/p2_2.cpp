#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <fstream>


#define input_file "number.txt"


main(int argc, char *argv[])
{
		int npes, myrank;

		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &npes);  // total number of processes
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
		
		MPI_Status status;

		int Start;
		int End;
		int sum = 0 ;
		int sum1 = 0 ;
		int a[64];
		if (myrank == 0){
			std::ifstream file;
			file.open(input_file);

			for (int i = 0; i < 64; i++){
				file >> a[i];
			}
			//MPI_Bcast(a, 64, MPI_INT, 0, MPI_COMM_WORLD);
		}
		MPI_Bcast(a, 64, MPI_INT, 0, MPI_COMM_WORLD);
		if (myrank == 0){
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			//std::cout << Start << End << std::endl;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}
			
		}
		else if (myrank == 1){
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}	

		}
		else if (myrank == 2){
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}	

		}
		else{
			Start = (64/4)*myrank;
			End = (64/4)*(myrank+1) - 1;
			for (int i = Start; i <= End; i++){
				sum = sum + a[i];
			}
			
		}
		MPI_Reduce(&sum,&sum1,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
		
		if (myrank == 0){
			std::cout << "The final sum is " << sum1 << std::endl;
		}

		MPI_Finalize();
}