#pragma once

#include <string>

class BoardCell
{
public:
	BoardCell();
	~BoardCell();

	bool mValue;
	BoardCell* mNeighbors[8];
};

class Board
{
public:
	Board();
	Board(int width, int height);
	~Board();
	
	void WriteLine(int height, const std::string& line);
	const BoardCell* GetBoard() const;
	void PlayRound();
	bool CheckNextState(const BoardCell* currentCell);

	int mWidth;
	int mHeight;
	int mSize;
protected:
	BoardCell* mBoard;
	BoardCell* mTempBoard;
};