all:
	clang++ single_core/timing.cpp single_core/board.cpp single_core/single_core.cpp -O3 -Wall -Werror -Wextra -pedantic -std=c++17 -o GameOfLifeSC