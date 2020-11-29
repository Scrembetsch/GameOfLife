void kernel gol_generation(global int* board, global int* tempBoard, int useTemp, int width, int height)
{
    int x = get_global_id(0);
    int y = get_global_id(1);
    int id = x + width * y;

    int y_ = y - 1;
    y_ += (y_ < 0) * height;
    int x_ = x - 1;
    x_ += (x_ < 0) * width;
    int y1 = y + 1;
    y1 -= (y1 >= height) * height;
    int x1 = x + 1;
    x1 -= (x1 >= width) * width;

    if(useTemp > 0)
    {
        int livingNeighbors = tempBoard[id];
        livingNeighbors += tempBoard[x_ + width * y_];
        livingNeighbors += tempBoard[x + width * y_];
        livingNeighbors += tempBoard[x1 + width * y_];

        livingNeighbors += tempBoard[x_ + width * y];
        livingNeighbors += tempBoard[x1 + width * y];

        livingNeighbors += tempBoard[x_ + width * y1];
        livingNeighbors += tempBoard[x + width * y1];
        livingNeighbors += tempBoard[x1 + width * y1];

        board[id] = (livingNeighbors == 3 + tempBoard[id] * (livingNeighbors == 4)) > 0;
    }
    else
    {
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

}
