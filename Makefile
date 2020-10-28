all:
	clang++ single_core/timing.cpp single_core/GoL.cpp -O3 -Wall -Werror -Wextra -pedantic -std=c++17 -o GoL