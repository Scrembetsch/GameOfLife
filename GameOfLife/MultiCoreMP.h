#pragma once

#include <string>

namespace MP
{
	extern int mNumGenerations;
	extern std::string mInputFile;
	extern std::string mOutputFile;
	extern int mDevice;

	extern bool* mBoard;
	extern bool* mTempBoard;
	extern bool** mBoardNeighbors;
	extern bool** mTempBoardNeighbors;
	extern int mWidth;
	extern int mHeight;
	extern int mSize;

	int PlayGame(const std::string& inputFile, const std::string& saveFile, int generations, int numThreads);

	bool InitBoard();
	bool CalcGenerations();
	bool PrintBoard();
}