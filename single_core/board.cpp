#include "board.h"

BoardCell::BoardCell()
	: mValue(false)
	, mNeighbors()
{
}

BoardCell::~BoardCell()
{
}

Board::Board()
	: Board(0, 0)
{
}

Board::Board(int width, int height)
	: mWidth(width)
	, mHeight(height)
	, mBoard(new BoardCell[width *  height])
	, mTempBoard(new BoardCell[width *  height])
{
}

Board::~Board()
{
}

void Board::WriteLine(int height, const std::string& line)
{
	for (int i = 0; i < line.size(); i++)
	{
		BoardCell bc;
		BoardCell tbc;
		switch (line[i])
		{
			default:
				return;

			case 'x':
				bc.mValue = true;
				break;

			case '.':
				bc.mValue = false;
				break;
		}

		int y_ = (height - 1 + mHeight) % mHeight;
		int x_ = (i - 1 + mWidth) % mWidth;
		int y1 = (height + 1 + mHeight) % mHeight;
		int x1 = (i + 1 + mWidth) % mWidth;

		bc.mNeighbors[0] = &mBoard[x_ + mWidth * y_];
		bc.mNeighbors[1] = &mBoard[i + mWidth * y_];
		bc.mNeighbors[2] = &mBoard[x1 + mWidth * y_];
		bc.mNeighbors[3] = &mBoard[x_ + mWidth * height];
		bc.mNeighbors[4] = &mBoard[x1 + mWidth * height];
		bc.mNeighbors[5] = &mBoard[x_ + mWidth * y1];
		bc.mNeighbors[6] = &mBoard[i + mWidth * y1];
		bc.mNeighbors[7] = &mBoard[x1 + mWidth * y1];

		tbc.mNeighbors[0] = &mTempBoard[x_ + mWidth * y_];
		tbc.mNeighbors[1] = &mTempBoard[i + mWidth * y_];
		tbc.mNeighbors[2] = &mTempBoard[x1 + mWidth * y_];
		tbc.mNeighbors[3] = &mTempBoard[x_ + mWidth * height];
		tbc.mNeighbors[4] = &mTempBoard[x1 + mWidth * height];
		tbc.mNeighbors[5] = &mTempBoard[x_ + mWidth * y1];
		tbc.mNeighbors[6] = &mTempBoard[i + mWidth * y1];
		tbc.mNeighbors[7] = &mTempBoard[x1 + mWidth * y1];

		mBoard[i + mWidth * height] = bc;
		mTempBoard[i + mWidth * height] = tbc;
	}
}

void Board::PlayRound()
{
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			int currentCell = i + mWidth * j;
			//mTempBoard[currentCell].mValue = Check(i, j);
			mTempBoard[currentCell].mValue = CheckNextState(&mBoard[currentCell]);
		}
	}
	std::swap(mTempBoard, mBoard);
}

bool Board::Check(int w, int h)
{
	const int x[] = {-1, -1, -1,  1, 1, 1,  0, 0};// 8 shifts to neighbors
	const int y[] = {-1,  0,  1, -1, 0, 1, -1, 1};// used in functions 
	int livingNeighbors = 0;
	for (int i = 0; i < 8; ++i)// visit the 8 spaces around it
	{
		int newX = (w + x[i] + mWidth) % mWidth;
		int newY = (h + y[i] + mHeight) % mHeight;
		livingNeighbors += mBoard[(newX) + mWidth * (newY)].mValue ? 1 : 0;
	}

	switch (livingNeighbors)
	{
		default:
			return false;

		case 2:
			return mBoard[w + mWidth * h].mValue;

		case 3:
			return true;

	}
	return true;
}

bool Board::CheckNextState(const BoardCell* currentCell)
{
	int livingNeighbors = 0;
	for (int i = 0; i < 8; i++)
	{
		livingNeighbors += (currentCell->mNeighbors[i]->mValue ? 1 : 0);
	}
	switch (livingNeighbors)
	{
		default:
			return false;

		case 2:
			return currentCell->mValue;

		case 3:
			return true;

	}
	return false;
}

const BoardCell* Board::GetBoard() const
{
	return mBoard;
}