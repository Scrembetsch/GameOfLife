#include "board.h"

Board::Board()
	: Board(0, 0)
{
}

Board::Board(int width, int height)
	: mWidth(width)
	, mHeight(height)
	, mBoard(new bool[width *  height])
	, mTempBoard(new bool[width *  height])
{
}

Board::~Board()
{
}

void Board::WriteLine(int height, const std::string& line)
{
	for (int i = 0; i < line.size(); i++)
	{
		switch (line[i])
		{
			default:
				break;

			case 'x':
				mBoard[i + mWidth * height] = true;
				break;

			case '.':
				mBoard[i + mWidth * height] = false;
				break;
		}
	}
}

void Board::PlayRound()
{
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			int currentCell = i + mWidth * j;
			mTempBoard[currentCell] = CheckNeighbors(currentCell);
		}
	}
}

bool Board::CheckNeighbors(int currentCell)
{
	return true;
}

const bool* Board::GetBoard() const
{
	return mBoard;
}