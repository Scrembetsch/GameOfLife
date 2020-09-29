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
	bool Check(int w, int h);
	bool InField(int w, int h);

	int mWidth;
	int mHeight;
protected:
	BoardCell* mBoard;
	BoardCell* mTempBoard;
};