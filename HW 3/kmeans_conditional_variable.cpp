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
#include <vector>

#define h  800 
#define w  800
#define NUM_CLUSTERS 6

#define input_file  "input.raw"
#define output_file "output.raw"

pthread_mutex_t r_lock;
pthread_cond_t r_cv;

int mean[] = {0,65,100,125,190,255};
int r=0;

struct thread_data {
	int thread_id;
	int NUM_PROCESSORS;
	std::vector< std::vector<int> > *Sum1;//Sum1 of elements of individual clusters 
	std::vector< std::vector<int> > *num_elements_per_clusters;//Elements assined to each cluster  
	unsigned char *A;//Input matrix 
	//std::vector<std::vector<float> > vector;
};

void *Kmeans(void *threadarg){
	struct  thread_data * my_data;
	my_data = (struct thread_data *) threadarg;
	int  thread_id = my_data->thread_id;  
	int p = my_data->NUM_PROCESSORS;
	auto &Sum1 = *my_data->Sum1;
	auto &NumElementsPerCluster = *my_data->num_elements_per_clusters;
	unsigned char *a = my_data->A;

	int start = thread_id*(h/p);
	int end = (thread_id + 1)*(h/p) - 1;
	

	for (int m =0 ; m < 50; m++){
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
				//pthread_mutex_lock(&mutex);
				Sum1[min_index][thread_id] = Sum1[min_index][thread_id] + a[i*h+j];
				//std::cout << Sum1[min_index][thread_id] << std::endl;
				NumElementsPerCluster[min_index][thread_id] = NumElementsPerCluster[min_index][thread_id] + 1;
				//pthread_mutex_unlock(&mutex);
			}
		}
		pthread_mutex_lock(&r_lock);
		if (r < p-1) {
			r++;
			//std::cout << r << std::endl;
			pthread_cond_wait(&r_cv, &r_lock);
		}
		if (r == p-1) {
			// recompute each mean (acording to slide)
			float Sum1K = 0;
			int len = 0;
			for (int k = 0; k < 6; k++){
				Sum1K = 0;
				len = 0;
				for (int u = 0; u < p; u++){
				Sum1K = Sum1K + accumulate(Sum1[k].begin(),Sum1[k].end(),0);// Sum1K + Sum1[k][p];
				len = len + accumulate(NumElementsPerCluster[k].begin(), NumElementsPerCluster[k].end(),0);
			}
			mean[k] = Sum1K/len;
		}
		r = 0;
		for (int k = 0; k < 6; k++){
		for (int y = 0; y < p;y++){
			Sum1[k][y] = 0;
			NumElementsPerCluster[k][y] = 0;
		}
		}
		pthread_cond_broadcast(&r_cv);
		}
		pthread_mutex_unlock(&r_lock);
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
	//pthread_mutex_init(&mutex,NULL);

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
	std::vector< std::vector<int> > Sum1(6, std::vector<int> (NUM_THREADS, 0));
	std::vector< std::vector<int> > NumelementsCluster(6, std::vector<int> (NUM_THREADS, 0));
	for (int k = 0; k < NUM_THREADS;k++){
		thread_data_array[k].thread_id = k;
		thread_data_array[k].NUM_PROCESSORS = NUM_THREADS;
		thread_data_array[k].Sum1 = &Sum1;
		thread_data_array[k].num_elements_per_clusters = &NumelementsCluster;
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