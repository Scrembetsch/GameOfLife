void kernel gol_generation(global int* board, global int* tempBoard, int width, int height)
{
    int id = get_global_id(0);
    int x = id % width;
    int y = id / width;

    int y_ = y - 1;
    y_ += (y_ < 0) * height;
    int x_ = x - 1;
    x_ += (x_ < 0) * width;
    int y1 = y + 1;
    y1 -= (y1 >= height) * height;
    int x1 = x + 1;
    x1 -= (x1 >= width) * width;

    int livingNeighbors = board[id];
    livingNeighbors += board[x_ + width * y_];
    livingNeighbors += board[x + width * y_];
    livingNeighbors += board[x1 + width * y_];

    livingNeighbors += board[x_ + width * y];
    livingNeighbors += board[x1 + width * y];

    livingNeighbors += board[x_ + width * y1];
    livingNeighbors += board[x + width * y1];
    livingNeighbors += board[x1 + width * y1];

    tempBoard[id] = (livingNeighbors == 3 + board[id] * (livingNeighbors == 4)) > 0;
}