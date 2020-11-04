all:
	clang++ single_core/*.cpp -O3 -Wall -Werror -Wextra -pedantic -std=c++17 -o GoL