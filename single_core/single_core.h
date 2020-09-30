#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Timing.h"

class BoardCell
{
public:
	BoardCell();
	~BoardCell();

	bool mValue;
	BoardCell* mNeighbors[8];
};

std::string mPath;
std::string* mSelectedFile;
BoardCell* mBoard;
BoardCell* mTempBoard;
int mWidth = 0;
int mHeight = 0;
int mSize = 0;

int main(int argc, char** argv);
bool ValidateArguments(int argc, char** argv);
bool InitBoard();
bool PrintBoard();
bool GetFilenames(const std::string& path, std::vector<std::string>& filenames);
