#pragma once

#include <string>

enum Mode
{
	SEQ,
	OMP,
	OCL
};

enum Device
{
	CPU,
	GPU
};

int mNumGenerations;
std::string mInputFile;
std::string mOutputFile;
bool mPrintMeasurment = false;
Mode mMode;
int mOmpThreads;
Device mDevice;

int main(int argc, char** argv);
bool LoadArguments(int argc, char** argv);
