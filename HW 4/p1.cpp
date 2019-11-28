#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>


main(int argc, char *argv[])
{
		int npes;
		int myrank;
		
		MPI_Init(&argc, &argv);
		
		MPI_Comm_size(MPI_COMM_WORLD, &npes);  // total number of processes
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
		MPI_Status status;
		//printf("From process %d out of %d, Hello World!\n",myrank, npes);
		int Msg;
		if (myrank == 0){
			Msg = 451;
			MPI_Send(&Msg,1,MPI_INT,1,200,MPI_COMM_WORLD);//200 is MPI_TAG
			std::cout << "Process 0 :" << " Initially Msg = " << Msg << std::endl;
			MPI_Recv(&Msg, 1, MPI_INT, 3, 200, MPI_COMM_WORLD, &status);
			std::cout << "Process 0 :" << " Recieved Msg = " << Msg << " Done!" << std::endl;

		}
		else if (myrank == 1){
			MPI_Recv(&Msg, 1, MPI_INT, 0, 200, MPI_COMM_WORLD,&status);
			Msg = Msg + 1;
			std::cout << "Process 1 :" << " Msg = " << Msg << std::endl;
			MPI_Send(&Msg,1,MPI_INT,2,200,MPI_COMM_WORLD);
			

		}
		else if (myrank == 2){
			MPI_Recv(&Msg, 1, MPI_INT, 1, 200, MPI_COMM_WORLD,&status);
			Msg = Msg + 1;
			std::cout << "Process 2 :" << " Msg = " << Msg << std::endl;
			MPI_Send(&Msg,1,MPI_INT,3,200,MPI_COMM_WORLD);

		}
		else{
			MPI_Recv(&Msg, 1, MPI_INT, 2, 200, MPI_COMM_WORLD,&status);
			Msg = Msg + 1;
			std::cout << "Process 3 :" << " Msg = " << Msg << std::endl;
			MPI_Send(&Msg,1,MPI_INT, 0, 200, MPI_COMM_WORLD);
		}
		

		MPI_Finalize();
}
