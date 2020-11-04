#pragma once

#include <string>

int mNumGenerations;
std::string mInputFile;
std::string mOutputFile;
bool mPrintMeasurment = false;

int main(int argc, char** argv);
bool LoadArguments(int argc, char** argv);
