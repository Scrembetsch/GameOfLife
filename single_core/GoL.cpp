#include <fstream>
#include <filesystem>
#include <iostream>

#include "GoL.h"
#include "Timing.h"

int main(int argc, char** argv)
{
    // Handle arguments
    if (!LoadArguments(argc, argv))
    {
        return -1;
    }

    InitBoard();
    CalcGenerations();
    PrintBoard();

    if (mPrintMeasurment)
    {
        Timing::getInstance()->print(true);
    }
    else
    {
        std::cout << "Finished" << std::endl;
    }
    return 0;
} 

bool LoadArguments(int argc, char** argv)
{
    bool noError = true;
    bool outputProvided = false;
    bool generationsProvided = false;

    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "-h")
        {
            std::cout << "Arguments:" << std::endl;
            std::cout << "\t--load <filename>\t Program loads this file (Is Required)" << std::endl;
            std::cout << "\t--save <filename>\t Saves the output in this file (Default: <loadFilename>_out.gol" << std::endl;
            std::cout << "\t--generations <n>\t Game plays this number of generations (Default: 250)" << std::endl;
            std::cout << "\t--measure\t\t Prints the setup, computation and finalization time" << std::endl;
            break;
        }
        if (arg == "--load")
        {
            if ((i + 1) < argc)
            {
                mInputFile = std::string(argv[i + 1]);
                std::ifstream inFile(mInputFile);
                if (!inFile.good())
                {
                    std::cout << "InputFile doesn't exist!" << std::endl;
                    noError = false;
                }
            }
            else
            {
                std::cout << "No Input file provided!" << std::endl;
                noError = false;
            }
        }
        else if (arg == "--save")
        {
            if ((i + 1) < argc)
            {
                mOutputFile = std::string(argv[i + 1]);
                outputProvided = true;
            }
            else
            {
                std::cout << "No Output filename provided!" << std::endl;
                noError = false;
            }
        }
        else if (arg == "--generations")
        {
            if ((i + 1) < argc)
            {
                mNumGenerations = std::stoi(argv[i + 1]);
                generationsProvided = true;
            }
            else
            {
                std::cout << "No generations provided provided!" << std::endl;
                noError = false;
            }
        }
        else if (arg == "--measure")
        {
            mPrintMeasurment = true;
        }
    }
    if (mInputFile.empty())
    {
        std::cout << "No Input file provided!" << std::endl;
        noError = false;
    }
    else if(!outputProvided)
    {
        mOutputFile = mInputFile.substr(0, mInputFile.length() - 4) + "_out.gol";
        std::cout << "No save location provided. Using default: " << mOutputFile << std::endl;
    }
    if (!generationsProvided)
    {
        mNumGenerations = 250;
        std::cout << "No number of Generations provided. Using default: " << mNumGenerations << std::endl;
    }
    return noError;
}

bool InitBoard()
{
    Timing::getInstance()->startSetup();
    std::ifstream boardFile(mInputFile);
    
    std::string line;
    char comma;
    boardFile >> mWidth;
    boardFile >> comma;
    boardFile >> mHeight;

    mSize = mWidth * mHeight;

    mBoard = new bool[mSize];
    mTempBoard = new bool[mSize];
    mBoardNeighbors = new bool* [mSize * 8];
    mTempBoardNeighbors = new bool* [mSize * 8];
    int lineCounter = 0;
    std::getline(boardFile, line);
    while (std::getline(boardFile, line))
    {
        for (uint32_t i = 0; i < line.size(); i++)
        {
            bool bc = line[i] == 'x';
            bool tbc = false;

            int pos = i + mWidth * lineCounter;

            int y_ = (lineCounter - 1 + mHeight) % mHeight;
            int x_ = (i - 1 + mWidth) % mWidth;
            int y1 = (lineCounter + 1 + mHeight) % mHeight;
            int x1 = (i + 1 + mWidth) % mWidth;

            mBoardNeighbors[0 + 8 * pos] = &mBoard[x_ + mWidth * y_];
            mBoardNeighbors[1 + 8 * pos] = &mBoard[i + mWidth * y_];
            mBoardNeighbors[2 + 8 * pos] = &mBoard[x1 + mWidth * y_];
            mBoardNeighbors[3 + 8 * pos] = &mBoard[x_ + mWidth * lineCounter];
            mBoardNeighbors[4 + 8 * pos] = &mBoard[x1 + mWidth * lineCounter];
            mBoardNeighbors[5 + 8 * pos] = &mBoard[x_ + mWidth * y1];
            mBoardNeighbors[6 + 8 * pos] = &mBoard[i + mWidth * y1];
            mBoardNeighbors[7 + 8 * pos] = &mBoard[x1 + mWidth * y1];

            mTempBoardNeighbors[0 + 8 * pos] = &mTempBoard[x_ + mWidth * y_];
            mTempBoardNeighbors[1 + 8 * pos] = &mTempBoard[i + mWidth * y_];
            mTempBoardNeighbors[2 + 8 * pos] = &mTempBoard[x1 + mWidth * y_];
            mTempBoardNeighbors[3 + 8 * pos] = &mTempBoard[x_ + mWidth * lineCounter];
            mTempBoardNeighbors[4 + 8 * pos] = &mTempBoard[x1 + mWidth * lineCounter];
            mTempBoardNeighbors[5 + 8 * pos] = &mTempBoard[x_ + mWidth * y1];
            mTempBoardNeighbors[6 + 8 * pos] = &mTempBoard[i + mWidth * y1];
            mTempBoardNeighbors[7 + 8 * pos] = &mTempBoard[x1 + mWidth * y1];

            mBoard[pos] = bc;
            mTempBoard[pos] = tbc;
        }
        ++lineCounter;
    }

    boardFile.close();
    Timing::getInstance()->stopSetup();
    return true;
}

bool CalcGenerations()
{
    Timing::getInstance()->startComputation();
    for (int i = 0; i < mNumGenerations; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            int livingNeighbors = 0;
            for (int k = 0; k < 8; k++)
            {
                livingNeighbors += *mBoardNeighbors[k + 8 * j];
            }
            mTempBoard[j] = livingNeighbors == 3 || (livingNeighbors == 2 && mBoard[j]);
        }
        std::swap(mTempBoard, mBoard);
        std::swap(mTempBoardNeighbors, mBoardNeighbors);
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
        }
        boardFile << std::endl;
    }
    boardFile.flush();

    boardFile.close();
    Timing::getInstance()->stopFinalization();
    return true;
}

