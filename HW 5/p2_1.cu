#include <stdlib.h>
#include <stdio.h>
#include <cublas.h>
#include <time.h>

#define size 1024


__global__ void mat_mul(int *a, int *b, int *c){
	int my_x,my_y;
	my_x = blockIdx.x*blockDim.x + threadIdx.x;
	my_y = blockIdx.y*blockDim.y + threadIdx.y;
	int local_c = 0;
	for(int i =0; i< size;i++){
		local_c = local_c +  a[my_x*size + i]*b[i*size + my_y]; 
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
		
		dim3 dimGrid(64,64);
		dim3 dimBlock(16,16);
		
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
