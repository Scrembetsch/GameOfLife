#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "SingleCore.h"
#include "Timing.h"

namespace SC
{
    int mNumGenerations;
    std::string mInputFile;
    std::string mOutputFile;

    bool* mBoard;
    bool* mTempBoard;
    bool** mBoardNeighbors;
    bool** mTempBoardNeighbors;
    int mWidth;
    int mHeight;
    int mSize;

    int PlayGame(const std::string& inputFile, const std::string& saveFile, int generations)
    {
        mInputFile = inputFile;
        mOutputFile = saveFile;
        mNumGenerations = generations;

        InitBoard();
        CalcGenerations();
        PrintBoard();

        return 0;
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

                int y_ = lineCounter - 1;
                y_ += (y_ < 0) * mHeight;
                int x_ = i - 1;
                x_ += (x_ < 0) * mWidth;
                int y1 = lineCounter + 1;
                y1 -= (y1 >= mHeight) * mHeight;
                int x1 = i + 1;
                x1 -= (x1 >= mWidth) * mWidth;

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
        for (int i = 0; i < mNumGenerations; ++i)
        {
            for (int j = 0; j < mSize; ++j)
            {
                int livingNeighbors = mBoard[j];
                bool** k = mBoardNeighbors + j * 8;
                bool** offset1 = k + 8;
                for (; k < offset1; ++k)
                {
                    livingNeighbors += **k;
                }

                mTempBoard[j] = livingNeighbors == 3 + mBoard[j] * (livingNeighbors == 4);
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
}