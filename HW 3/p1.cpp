#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <iostream>


#define	num_of_points 40000000
typedef struct{
	double x;  
	double y;
}Point; 

int main(void){
	int i;
	//int num_of_points_in_circle;
	double pi;
	struct timespec start, stop; 
	double time;
	int tid;
	Point * data_point = (Point *) malloc (sizeof(Point)*num_of_points);
	for(i=0; i<num_of_points; i++){
		data_point[i].x=(double)rand()/(double)RAND_MAX;
		data_point[i].y=(double)rand()/(double)RAND_MAX;
	}

	int num_of_points_in_circle[4];
	
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	
	
	////////**********Use OpenMP to parallize this loop***************//
	int chunk = 10000000;
	omp_set_num_threads(4);
	#pragma omp parallel shared(num_of_points_in_circle,data_point,chunk) private(i,tid) 
	{
		tid = omp_get_thread_num();
		#pragma omp for schedule(static,chunk) ordered
			for(i=0; i<num_of_points; i++){
				if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25){
					num_of_points_in_circle[tid]++;
				}	
				//std::cout << tid << std::endl;
			}	
	}
	///////******************************////
	int sum;
		for (int j = 0; j < 4;j++){
			sum = sum + num_of_points_in_circle[j];
		}

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	//std::cout << num_of_points_in_circle << std::endl;
	pi =4*(double)sum/(double)num_of_points;
	printf("Estimated pi is %f, execution time = %f sec\n",  pi, time);		
	//free(num_of_points_in_circle);
	//return 0;
}	