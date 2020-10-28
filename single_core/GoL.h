#pragma once

#include <string>

int mNumGenerations;
std::string mInputFile;
std::string mOutputFile;
bool mPrintMeasurment = false;

bool* mBoard;
bool* mTempBoard;
bool** mBoardNeighbors;
bool** mTempBoardNeighbors;
int mWidth = 0;
int mHeight = 0;
int mSize = 0;

int main(int argc, char** argv);
bool LoadArguments(int argc, char** argv);
bool InitBoard();
bool CalcGenerations();
bool PrintBoard();
