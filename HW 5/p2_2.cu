#include <stdlib.h>
#include <stdio.h>
#include <cublas.h>
#include <time.h>

#define size 1024
#define blocksize 32

__global__ void mat_mul(int *a, int *b, int *c){
	int my_x;
	int my_y;
	int row = threadIdx.y;
	int col = threadIdx.x;
	my_x = blockIdx.x*blockDim.x + threadIdx.x;
	my_y = blockIdx.y*blockDim.y + threadIdx.y;

	__shared__ float A_share[blocksize][blocksize];
	__shared__ float B_share[blocksize][blocksize];

	int local_c = 0;

	for(int i = 0; i < size/blocksize;i++){
		A_share[row][col] = a[my_x*size + (i*blockDim.y + col)];
		B_share[row][col] = b[(i*blockDim.x + row)*size + my_y];
		__syncthreads();
		for(int j =0; j< blocksize;j++){
			local_c += A_share[row][j]*B_share[j][col]; 
		}
		__syncthreads();
	}
	c[my_x*size + my_y] = local_c;  
}

int main(){		
    int i;
    int *a = (int*)malloc(sizeof(int)*size*size);          
    int *b = (int*)malloc(sizeof(int)*size*size);          
    int *c = (int*)malloc(sizeof(int)*size*size);           	
	
	  for(i=0; i<size*size; i++){
			a[i]=1;
			b[i]=2;
  	}
		int *gpu_a, *gpu_b, *gpu_c;
		cudaMalloc((void**)&gpu_a, sizeof(int)*size*size); 
		cudaMalloc((void**)&gpu_b, sizeof(int)*size*size);
		cudaMalloc((void**)&gpu_c, sizeof(int)*size*size);
		
		struct timespec start, stop; 
	    double time;
	  
	  
		cudaMemcpy(gpu_a, a, sizeof(int)*size*size, cudaMemcpyHostToDevice);
		cudaMemcpy(gpu_b, b, sizeof(int)*size*size, cudaMemcpyHostToDevice);
		
		dim3 dimGrid(32,32);
		dim3 dimBlock(32,32);
		
		if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) { perror( "clock gettime" );}
		mat_mul<<<dimGrid, dimBlock>>>(gpu_a, gpu_b, gpu_c);				
		cudaMemcpy(c, gpu_c, sizeof(int)*size*size, cudaMemcpyDeviceToHost);
		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror( "clock gettime" );}	  
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		printf("time is %f ns\n", time*1e9);	 
		
		printf("C[%d][%d] = %d ",451,451,c[451*size +451]);
  	
		free(a);
		free(b);
		free(c);
		cudaFree(gpu_a);  
		cudaFree(gpu_b);  
		cudaFree(gpu_c);  
		return 0;
}	
