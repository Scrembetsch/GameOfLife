#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "../timing/Timing.h"

std::string mPath;
std::string* mSelectedFile;
Board mBoard;

int main(int argc, char** argv);
bool ValidateArguments(int argc, char** argv);
bool InitBoard();
bool PrintBoard();
bool GetFilenames(const std::string& path, std::vector<std::string>& filenames);
