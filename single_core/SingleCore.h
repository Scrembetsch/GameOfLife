#pragma once

#include <string>

namespace SC
{
	extern int mNumGenerations;
	extern std::string mInputFile;
	extern std::string mOutputFile;

	extern bool* mBoard;
	extern bool* mTempBoard;
	extern bool** mBoardNeighbors;
	extern bool** mTempBoardNeighbors;
	extern int mWidth;
	extern int mHeight;
	extern int mSize;

	int PlayGame(const std::string& inputFile, const std::string& saveFile, int generations);

	bool InitBoard();
	bool CalcGenerations();
	bool PrintBoard();
}