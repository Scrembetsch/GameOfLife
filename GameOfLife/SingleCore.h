#pragma once

#include <string>

#define ARR_TYPE int_fast8_t
namespace SC
{
	extern int mNumGenerations;
	extern std::string mInputFile;
	extern std::string mOutputFile;

	extern ARR_TYPE* mBoard;
	extern ARR_TYPE* mTempBoard;
	extern int mWidth;
	extern int mHeight;
	extern int mSize;

	int PlayGame(const std::string& inputFile, const std::string& saveFile, int generations);

	bool InitBoard();
	bool CalcGenerations();
	bool PrintBoard();
}