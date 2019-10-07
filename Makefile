

#p1: problem1_trial.cpp
#	g++ problem1_trial.cpp -o p1

#p1b: block_matrix.cpp 
#	g++ block_matrix.cpp -o p1b

#p2: kmeans_problem.cpp
#	g++ kmeans_problem.cpp -o p2

#clean:
#	rm p1 p1b p2

p1a p1b p2: problem1_trial.cpp block_matrix.cpp kmeans_problem.cpp
	g++ problem1_trial.cpp -o p1a
	g++ block_matrix.cpp -o p1b
	g++ kmeans_problem.cpp -o p2 -std=c++11
