#pragma once

#include <string>
#include <vector>

#if defined(__APPLE__) || defined(__MACOSX)
	#include <OpenCL/cl.hpp>
#else
	#include <CL/cl.hpp>
#endif


namespace CL
{
	extern int mNumGenerations;
	extern std::string mInputFile;
	extern std::string mOutputFile;
	extern int mNumThreads;

	extern int* mBoard;
	extern int* mTempBoard;
	extern int mWidth;
	extern int mHeight;
	extern int mSize;

	extern const std::string KERNEL_FILE;
	extern cl::Program mProgram;
	extern cl::Buffer mBoardBuffer;
	extern cl::Buffer mTempBoardBuffer;
	extern cl::Kernel mGolKernel;
	extern cl::CommandQueue mQueue;

	int PlayGame(const std::string& inputFile, const std::string& saveFile, int generations, int device);

	bool Init();
	bool InitOpenCL();
	bool InitBoard();
	bool CalcGenerations();
	bool PrintBoard();
}