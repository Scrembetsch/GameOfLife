#include "board.h"

Board::Board()
	: Board(0, 0)
{
}

Board::Board(int width, int height)
	: mWidth(width)
	, mHeight(height)
	, mBoard(new bool[width *  height])
{
}

Board::~Board()
{
	delete[] mBoard;
}

void Board::WriteLine(int height, const std::string& line)
{
	for (int i = 0; i < line.size(); i++)
	{
		mBoard[i * mHeight + height] = line[i] == '#';
	}
}

const bool* Board::GetBoard() const
{
	return mBoard;
}