#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


struct  thread_data{
	int	thread_id;
	int  NUM_PROCESSORS; //Get the run time value of number of processors. 
	int  N; // NxN matrix
	//int *pointer;//
	double **Matrix_A;
	double **Matrix_B;
	double **Matrix_C;
};


void *MatrixMultiply(void *threadarg){
	struct  thread_data * my_data;
	my_data = (struct thread_data *) threadarg;

	int  thread_id = my_data->thread_id; 
	int n = my_data->N;
	int p = my_data->NUM_PROCESSORS;
	//int *pointer = my_data->pointer; //Potential error statement pointer syntax 
	double **A = my_data->Matrix_A;
	double **B = my_data->Matrix_B;
	double **C = my_data->Matrix_C;

	//pointer to start and end of the B matrix; this is unique for each thread 
	int start = thread_id*(n/p);
	int end = (thread_id + 1)*(n/p) - 1 ;

	 
	
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			for (int k=start;k<=end;k++){
				//C[i][k] = C[i][k] + A[i][j]*B[j][k];
				//*(C+(i*n+k)) = *(C+(i*n+k)) + *(A + (i*n+j))*(*(B + (j*n + k)));
				//(*(C+i)+k) += (*(A+i)+j)*((*(B+k)+j))
				C[i][k] = C[i][k] + A[i][j]*B[j][k];
				//std::cout << *(C+(i*n+k))<< std::endl;
				//std::cout <<  *(A + (i*n+j)) << std::endl;
				//std::cout << *(B + (j*n + k)) << std::endl;
			}
		}
	}
	
	//printf("Thread %d says %s  \n", thread_id , hello_msg);
	pthread_exit(NULL);
}

int  main(int argc, char *argv[]) {
			//Take the number of processors as a command line argument 
			int NUM_THREADS = atoi(argv[1]);
			int n = 4096; // matrix size is n*n
			pthread_t  threads[NUM_THREADS];
			int *thread_ids[NUM_THREADS];
			int rc;
			struct  thread_data  thread_data_array[NUM_THREADS];
			pthread_attr_t  attr;
			pthread_attr_init(&attr);

			//Initialize the matrices 
			int i, j, k;
			struct timespec start, stop; 
			double time;
			
			
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

			if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}// for reading time 

			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
			for(i=0; i<NUM_THREADS; i++) {	  
				thread_data_array[i].thread_id = i;
				thread_data_array[i].N = n;  
				thread_data_array[i].NUM_PROCESSORS = NUM_THREADS; 
				thread_data_array[i].Matrix_A = A;
				thread_data_array[i].Matrix_B = B;
				thread_data_array[i].Matrix_C = C;
				rc = pthread_create(&threads[i], &attr, MatrixMultiply, (void *) &thread_data_array[i] );
				if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}
			}
			pthread_attr_destroy(&attr);
			for(int t=0; t<NUM_THREADS; t++) {
      		rc = pthread_join(threads[t], NULL);
        	 	if (rc) { printf(" joining error %d ", rc);   exit(-1);}
        	 }

        	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
			float t = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
			std::cout << "Execution time (in secs) = " << t << std::endl;
			std::cout << C[100][100] << std::endl;

			pthread_exit(NULL);
}



