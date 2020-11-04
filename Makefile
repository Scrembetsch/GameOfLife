all:
	g++ single_core/*.cpp -fopenmp -O3 -Wall -Werror -Wextra -pedantic -std=c++17 -o GoL