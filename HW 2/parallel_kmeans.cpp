//K-MEANS CLUSTERING ALGORITHM 
//AUTHOR- ANIRUDH KULKARNI

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <pthread.h>

#define h  800 
#define w  800
#define NUM_CLUSTERS 6

#define input_file  "input.raw"
#define output_file "output.raw"
pthread_mutex_t mutex;
float mean[] = {0,65,100,125,190,255};

struct thread_data {
	int thread_id;
	int NUM_PROCESSORS;
	int *sum;//Sum of elements of individual clusters 
	int *num_elements_per_clusters;//Elements assined to each cluster  
	unsigned char *A;//Input matrix 
	//std::vector<std::vector<float> > vector;
};

void *Kmeans(void *threadarg){
	struct  thread_data * my_data;
	my_data = (struct thread_data *) threadarg;

	int  thread_id = my_data->thread_id;  
	int p = my_data->NUM_PROCESSORS;
	int *sum = my_data->sum;
	int *NumElementsPerCluster = my_data->num_elements_per_clusters;
	unsigned char *a = my_data->A;
	//std::vector<std::vector<float> > vector = my_data->vector;

	int start = thread_id*(h/p);
	int end = (thread_id + 1)*(h/p) - 1;


	for (int i = 0;i < w;i++){
		for (int j = start; j <= end; j++){
			//Using euclidean distance 
			//float d[] = {fabs(a[i*h+j] - mean[0]), fabs(a[i][j] - mean[1]), fabs(a[i][j] - mean[2]),fabs(a[i][j] - mean[3]), fabs(a[i][j] - mean[4]), fabs(a[i][j] - mean[5])};
			float d[6];
			d[0] = fabs(a[i*h+j] - mean[0]);
			d[1] = fabs(a[i*h+j] - mean[1]);
			d[2] = fabs(a[i*h+j] - mean[2]);
			d[3] = fabs(a[i*h+j] - mean[3]);
			d[4] = fabs(a[i*h+j] - mean[4]);
			d[5] = fabs(a[i*h+j] - mean[5]);
			int min_index;
			//cout << *(&d + 1) - d << endl; //size of an array 
			min_index = std::min_element(d+0,d+6) - d+0;
			//std::cout << min_index << std::endl;
			//cout << min_index << endl;
			pthread_mutex_lock(&mutex);
			sum[min_index] = sum[min_index] + a[i*h+j];
			NumElementsPerCluster[min_index] = NumElementsPerCluster[min_index] + 1;
			pthread_mutex_unlock(&mutex);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){

	//Read the matrix from raw file 
	int i;
	FILE *fp;
	int NUM_THREADS = atoi(argv[1]);
	int iterations = 50;//Number of Iterations 
	
	pthread_t  threads[NUM_THREADS];
	int rc;
	struct  thread_data  thread_data_array[NUM_THREADS];
	pthread_attr_t  attr;
	pthread_attr_init(&attr);
	pthread_mutex_init(&mutex,NULL);

	unsigned char a[h][w]; 
	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		std::cout << "Cannot open file: " << input_file << std::endl;
		exit(1);
	}
	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);


	//Start counting time 
	struct timespec start, stop;
	double t;
	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
	{ 
		perror( "clock gettime" );
	}

	unsigned char *Matrix_A = &a[0][0];
	for (int i = 0; i < iterations ; i++){
		//std::vector<std::vector<float> > clusters (6, std::vector<float>());
		int sum[6] = {0};
		int NumelementsCluster[6] = {0};
		//Create threads 
		for (int k = 0; k < NUM_THREADS;k++){
			thread_data_array[k].thread_id = k;
			thread_data_array[k].NUM_PROCESSORS = NUM_THREADS;
			thread_data_array[k].sum = &sum[0];
			thread_data_array[k].num_elements_per_clusters = &NumelementsCluster[0];
			thread_data_array[k].A = Matrix_A;
			//thread_data_array[k].vector = clusters;
			rc = pthread_create(&threads[k], &attr, Kmeans, (void *) &thread_data_array[k] );
			//std::cout << rc << std::endl;
			if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}
		}

		//Join threads 
			for(int t=0; t<NUM_THREADS; t++) {
			rc = pthread_join(threads[t], NULL);
			//std::cout << rc << std::endl;
			if (rc) { printf(" joining error %d ", rc);   exit(-1);}
			}
		//Recomupte the means
		for (int k = 0; k < 6; k++){
			mean[k] = sum[k]/NumelementsCluster[k];
		}
		//clusters.clear(); 
		std::fill_n(sum, 6, 0);
		std::fill_n(NumelementsCluster,6,0);
	}

	//After 50 iterations, replace the elements with the mean of the cluster they belong to
	for (int i =0; i < w; i++){
		for (int j = 0; j < h; j++){
			float e[6];
			e[0] = (abs(a[i][j] - mean[0]));
			e[1] = (abs(a[i][j] - mean[1]));
			e[2] = (abs(a[i][j] - mean[2]));
			e[3] = (abs(a[i][j] - mean[3]));
			e[4] = (abs(a[i][j] - mean[4]));
			e[5] = (abs(a[i][j] - mean[5]));
			float index;
			index = std::min_element(e+0,e+6) - e+0;//Check if this is e+6 or e+5
			a[i][j] = int(mean[int(index)]);
		}
	}

	for (int j = 0; j < 6;j++){
		std::cout << mean[j] << std::endl;
	}
	//End counting time
	if(clock_gettime(CLOCK_REALTIME, &stop)  == -1)
	{
		perror( "clock gettime" );
	}
	t = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec)/1e9;
	std::cout << "Execution time in sec " << t << std::endl;


	//Write the matrix to output file
	if (!(fp=fopen(output_file,"wb"))) {
		std::cout << "Cannot open file: " << output_file << std::endl;
		exit(1);
	}
	fwrite(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);
	//std::cout << "Reached the End" << std::endl;
	pthread_exit(NULL);

}