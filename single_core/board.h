#pragma once

#include <string>

class Board
{
public:
	Board();
	Board(int width, int height);
	~Board();
	
	void WriteLine(int height, const std::string& line);
	const bool* GetBoard() const;

	int mWidth;
	int mHeight;
protected:
	bool* mBoard;
};