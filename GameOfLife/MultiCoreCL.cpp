#include <fstream>
#include <filesystem>
#include <iostream>

#include "MultiCoreCL.h"
#include "Timing.h"

#define __CL_ENABLE_EXCEPTIONS

namespace CL
{
    int mNumGenerations;
    std::string mInputFile;
    std::string mOutputFile;
    int mDevice;

    int* mBoard;
    int* mTempBoard;
    int mWidth;
    int mHeight;
    int mSize;

    const std::string KERNEL_FILE = "GoL.cl";
    cl::Program mProgram;
    cl::Buffer mBoardBuffer;
    cl::Buffer mTempBoardBuffer;
    cl::Kernel mGolKernel;
    cl::CommandQueue mQueue;

    int PlayGame(const std::string& inputFile, const std::string& saveFile, int generations, int device)
    {
        mInputFile = inputFile;
        mOutputFile = saveFile;
        mNumGenerations = generations;
        mDevice = device;

        Init();
        CalcGenerations();
        PrintBoard();

        return 0;
    }

    bool Init()
    {
        Timing::getInstance()->startSetup();
        
        InitBoard();
        InitOpenCL();

        Timing::getInstance()->stopSetup();
        return true;
    }
    bool InitOpenCL()
    {
        // Check avalable OpenCL Platforms
        std::vector<cl::Platform> all_platforms;
        cl::Platform::get(&all_platforms);
        if (all_platforms.size() == 0)
        {
            std::cout << " No platforms found. Check OpenCL installation!\n";
            exit(1);
        }
        // Use selected Platform -> Default: 0->GPU, 1->CPU
        cl::Platform default_platform = all_platforms[mDevice];
        //std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

        // Get the Devices of the selected platform (if PC has mutliple graphic cards)
        std::vector<cl::Device> all_devices;
        default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
        if (all_devices.size() == 0)
        {
            std::cout << " No devices found. Check OpenCL installation!\n";
            exit(1);
        }
        // Ignore multiple devices, Always tkae first
        cl::Device default_device = all_devices[0];
        //std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

        cl::Context context({default_device});
        cl::Program::Sources sources;

        // Load Kernel Program and compile it
        std::ifstream sourceFile(KERNEL_FILE);
        if (!sourceFile)
        {
            std::cout << "Kernel source file " << KERNEL_FILE << " not found!" << std::endl;
            return 1;
        }
        // Load file into string
        std::string sourceCode(
            std::istreambuf_iterator<char>(sourceFile),
            (std::istreambuf_iterator<char>()));

        sources.push_back({sourceCode.c_str(), sourceCode.length()});

        // Compile Kernel File
        cl::Program mProgram(context, sources);
        if (mProgram.build({default_device}) != CL_SUCCESS)
        {
            std::cout << " Error building: " << mProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
            exit(1);
        }

        // Init buffers
        mBoardBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * mSize);
        mTempBoardBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * mSize);

        mQueue = cl::CommandQueue(context, default_device);

        mGolKernel = cl::Kernel(mProgram, "gol_generation");

        mQueue.enqueueWriteBuffer(mBoardBuffer, CL_TRUE, 0, sizeof(int) * mSize, mBoard);
        mQueue.enqueueWriteBuffer(mTempBoardBuffer, CL_TRUE, 0, sizeof(int) * mSize, mTempBoard);
        return true;
    }

    bool InitBoard()
    {
        std::ifstream boardFile(mInputFile);

        std::string line;
        char comma;
        boardFile >> mWidth;
        boardFile >> comma;
        boardFile >> mHeight;

        mSize = mWidth * mHeight;

        mBoard = new int[mSize];
        mTempBoard = new int[mSize];

        int lineCounter = 0;
        std::getline(boardFile, line);
        while (std::getline(boardFile, line))
        {
            int lineSize = line.size();
            {
                for (int i = 0; i < lineSize; i++)
                {

                    int pos = i + mWidth * lineCounter;

                    mBoard[pos] = line[i] == 'x';
                    mTempBoard[pos] = false;
                }
            }
            ++lineCounter;
        }

        boardFile.close();
        return true;
    }

    bool CalcGenerations()
    {
        Timing::getInstance()->startComputation();

        // UseTemp is used to control if in CL, the if is used to avoid memory copy after each generation
        bool useTemp = true;
        for (int i = 0; i < mNumGenerations; i++)
        {
            useTemp = !useTemp;

            mGolKernel.setArg(0, mBoardBuffer);
            mGolKernel.setArg(1, mTempBoardBuffer);
            mGolKernel.setArg(2, cl_int(useTemp));
            mGolKernel.setArg(3, cl_int(mWidth));
            mGolKernel.setArg(4, cl_int(mHeight));
            mQueue.enqueueNDRangeKernel(mGolKernel, cl::NullRange, cl::NDRange(mSize), cl::NullRange);
            mQueue.finish();
        }
        if (useTemp)
        {
            mQueue.enqueueReadBuffer(mBoardBuffer, CL_TRUE, 0, sizeof(int) * mSize, mBoard);
        }
        else
        {
            mQueue.enqueueReadBuffer(mTempBoardBuffer, CL_TRUE, 0, sizeof(int) * mSize, mTempBoard);
            std::swap(mTempBoard, mBoard);
        }

        Timing::getInstance()->stopComputation();
        return true;
    }

    bool PrintBoard()
    {
        Timing::getInstance()->startFinalization();
        if (std::filesystem::exists(mOutputFile))
        {
            std::remove(mOutputFile.c_str());
        }
        std::ofstream boardFile(mOutputFile);

        boardFile << mWidth << "," << mHeight << std::endl;
        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                boardFile << (mBoard[x + mWidth * y] ? 'x' : '.');
                //boardFile << mBoard[x + mWidth * y] << ",";
            }
            boardFile << std::endl;
        }
        boardFile.flush();

        boardFile.close();
        Timing::getInstance()->stopFinalization();
        return true;
    }
}