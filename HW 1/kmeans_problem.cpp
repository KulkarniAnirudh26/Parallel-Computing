//K-MEANS CLUSTERING ALGORITHM 
//AUTHOR- ANIRUDH KULKARNI

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <time.h>
#include <algorithm>

#define h  800 
#define w  800

#define input_file  "input.raw"
#define output_file "output.raw"

using namespace std;

int main(int argc, char *argv[]){


	//Read the matrix from raw file 
	int i;
    FILE *fp;

  	unsigned char a[h][w]; 
	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		cout << "Cannot open file: " << input_file << endl;
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

	//Serial K-means algorithm
	int N = 4;//Number of clusters 
	int iterations = 30;//Number of Iterations 
	
	//Initialize means
	float mean[] = {0,85,170,255};
	for (int m = 0; m < 30; m++) {
		std::vector<std::vector<float> > clusters (4, std::vector<float>());
	//Calculate the distance of the means with respect to each point 
		for (int i = 0;i < w;i++){
			for (int j = 0; j < h; j++){
				//Using euclidean distance 
				float d[] = {fabs(a[i][j] - mean[0]), fabs(a[i][j] - mean[1]), fabs(a[i][j] - mean[2]),fabs(a[i][j] - mean[3])};
				float min_index;
				//cout << *(&d + 1) - d << endl; //size of an array 
				min_index = std::min_element(d+0,d+4) - d+0;
				//cout << min_index << endl;
				clusters[min_index].push_back(a[i][j]);
			}
		}
		//Recomupte the means
		for (int k = 0; k < 4; k++){
			float Nelements1 = clusters[k].size();
			float Nsum = accumulate(clusters[k].begin(), clusters[k].end(), 0);
			mean[k] = Nsum/Nelements1;
		}
		clusters.clear();
    }
    
	//After 30 iterations 
	//Replace inidividual points in the original matrix with the final means 
    for (int l = 0; l < 4; l++){
    	cout << mean[l] << "\t";
    }
	cout << "\n";


	for (int i =0; i < w; i++){
		for (int j = 0; j < h; j++){
			float e[] = {(abs(a[i][j] - mean[0])),(abs(a[i][j] - mean[1])),(abs(a[i][j] - mean[2])),(abs(a[i][j] - mean[3])) };
			float index;
			index = std::min_element(e+0,e+4) - e+0;
			a[i][j] = int(mean[int(index)]);
		}
	}

	//End counting time
	if(clock_gettime(CLOCK_REALTIME, &stop)  == -1)
	{
		perror( "clock gettime" );
	}
	t = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec)/1e9;
	cout << t << endl;


	//Write the matrix to output file
	if (!(fp=fopen(output_file,"wb"))) {
		cout << "Cannot open file: " << output_file << endl;
		exit(1);
	}
	fwrite(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);
	return 0;

}
