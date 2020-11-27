#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "SingleCore.h"
#include "Timing.h"

#define CALC_NEIGHBORS(board, width, living, x_, x, x1, y_, y, y2) {\
                    living = board[x_ + width * y_];\
                    living+= board[x + width * y_];\
                    living+= board[x1 + width * y_];\
                    living+= board[x_ + width * y];\
                    living+= board[x1 + width * y];\
                    living+= board[x_ + width * y2];\
                    living+= board[x + width * y2];\
                    living+= board[x1 + width * y2];\
                    }\

namespace SC
{
    int mNumGenerations;
    std::string mInputFile;
    std::string mOutputFile;

    ARR_TYPE* mBoard;
    ARR_TYPE* mTempBoard;
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

        mBoard = new ARR_TYPE[mSize];
        mTempBoard = new ARR_TYPE[mSize];
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
        int x = 0;
        int y = 0;
        int y_ = 0;
        int x_ = 0;
        int y1 = 0;
        int x1 = 0;
        int pos = 0;
        int mWidthSmall = mWidth - 1;
        int mHeightSmall = mHeight - 1;
        int mWidthMini = mWidthSmall - 1;
        int mHeightMini = mHeightSmall - 1;
        ARR_TYPE livingNeighbors = 0;
        ARR_TYPE* it = 0;
        ARR_TYPE* it2 = 0;
        Timing::getInstance()->startComputation();
        for (int i = 0; i < mNumGenerations; ++i)
        {
            // Inner cells
            for (y = 1; y < mHeightSmall; y++)
            {
                for (x = 1; x < mWidthSmall; x++)
                {
                    pos = x + mWidth * y;
                    it2 = &mBoard[pos];
                    it = it2 - mWidth - 1;

                    livingNeighbors = *it;
                    livingNeighbors += *(++it);
                    livingNeighbors += *(++it);

                    it = it - 2 + mWidth;
                    livingNeighbors += *(it);
                    it = it + 2;
                    livingNeighbors += *(it);

                    it = it - 2 + mWidth;
                    livingNeighbors += *(it);
                    livingNeighbors += *(++it);
                    livingNeighbors += *(++it);

                    mTempBoard[pos] = (livingNeighbors == 3) | (*it2 & (livingNeighbors == 2));
                }
            }
            // First row
            y1 = 1;
            y = 0;
            y_ = mHeightSmall;
            for (x = 1; x < mWidthSmall; x++)
            {
                pos = x + mWidth * y;

                x_ = x - 1;
                x1 = x + 1;

                CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, x_, x, x1, y_, y, y1);

                mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));
            }
            // Last row
            y_ = mHeightMini;
            y = mHeightSmall;
            y1 = 0;
            for (x = 1; x < mWidthSmall; x++)
            {
                x_ = x - 1;
                x1 = x + 1;

                pos = x + mWidth * mHeightSmall;

                CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, x_, x, x1, y_, y, y1);

                mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));
            }

            // NW
            pos = 0;
            CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, mWidthSmall, 0, 1, mHeightSmall, 0, 1);
            mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));

            // Left column
            x_ = mWidthSmall;
            x = 0;
            x1 = 1;
            for (y = 1; y < mHeightSmall; y++)
            {
                y_ = y - 1;
                y1 = y + 1;

                pos = x + mWidth * y;

                CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, x_, x, x1, y_, y, y1);

                mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));
            }

            // SW
            pos = mWidth * mHeightSmall;
            CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, mWidthSmall, 0, 1, mHeightMini, mHeightSmall, 0);
            mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));

            // NE
            pos = mWidthSmall;
            CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, mWidthMini, mWidthSmall, 0, mHeightSmall, 0, 1);
            mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));

            // Right column
            x_ = mWidthMini;
            x = mWidthSmall;
            x1 = 0;
            for (y = 1; y < mHeightSmall; y++)
            {
                y_ = y - 1;
                y1 = y + 1;

                pos = x + mWidth * y;

                CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, x_, x, x1, y_, y, y1);

                mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));
            }
            // SE
            pos = mWidthSmall + mWidth * mHeightSmall;
            CALC_NEIGHBORS(mBoard, mWidth, livingNeighbors, mWidthMini, mWidthSmall, 0, mHeightMini, mHeightSmall, 0);
            mTempBoard[pos] = (livingNeighbors == 3) | (mBoard[pos] & (livingNeighbors == 2));

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
            }
            boardFile << std::endl;
        }
        boardFile.flush();

        boardFile.close();
        Timing::getInstance()->stopFinalization();
        return true;
    }
}