#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <iostream>
#include <bits/stdc++.h> 


#define	size 16*1024*1024

void swap(int* a, int* b)  
{  
    int t = *a;  
    *a = *b;  
    *b = t;  
}  

int partition (int arr[], int start, int end)  
{  
	int pivot = arr[end]; // pivot  
    int i = (start - 1); // Index of smaller element  
  
    for (int j = start; j <= end - 1; j++)  
    {  
        // If current element is smaller than the pivot  
        if (arr[j] < pivot)  
        {  
            i++; // increment index of smaller element  
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[end]);  
    return (i + 1); 
}  

int partition_r(int arr[], int start, int end){
	int index = rand()%(end-start+1) + start;
	swap(&arr[index], &arr[end]); 
    return partition(arr, start, end); 
}
  
void quickSort(int arr[], int start, int end)  
{  
    if (start < end)  
    {  
        /* pi is partitioning index, arr[p] is now  
        at right place */
        int pi = partition_r(arr, start, end);  
        // Separately sort elements before  
        // partition and after partition  
        //for (int j = 0; j < 8; j++){
        //	std::cout << arr[j] << "\t";
        //}
        //std::cout << "\n";
        quickSort(arr, start, pi - 1);  
        quickSort(arr, pi + 1, end);  
    }  
}  

int main(void){
	int i, j, tmp;
	struct timespec start, stop; 
	double exe_time;
	int tid;
	srand(time(NULL)); 
	int * m = (int *) malloc (sizeof(int)*size);
	for(i=0; i<size; i++){
		m[i]=size-i;
	}
	
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	////////**********Your code goes here***************//
	int low=0;
	int high=size-1;
	int pivot = partition_r(m,0,size-1);
	omp_set_num_threads(2);
	#pragma omp parallel shared(m,pivot) private(tid)
	{
		tid  = omp_get_num_threads();
		#pragma omp sections
		{ 
			#pragma omp section 
			{
				quickSort(m,low,pivot-1);
			}
			#pragma omp section 
			{
				quickSort(m,pivot+1,high);
			}
		}
	}
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	
	for(i=0;i<16;i++) printf("%d ", m[i]);		
	printf("\nExecution time = %f sec\n",  exe_time);
	return 0;		
}	