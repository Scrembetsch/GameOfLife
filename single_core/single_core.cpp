#include "single_core.h"

BoardCell::BoardCell()
    : mValue(false)
    , mNeighbors()
{
}

BoardCell::~BoardCell()
{
}

int main(int argc, char** argv)
{
    // Handle arguments
    if (!ValidateArguments(argc, argv))
    {
        return -1;
    }

    // Show filenames
    std::vector<std::string> filenames;
    if (GetFilenames(mPath, filenames))
    {
        std::cout << "Select file:" << std::endl;
        for (uint32_t i = 0; i < filenames.size(); i++)
        {
            std::cout << "(" << i << ") " << filenames[i] << std::endl;
        }
        std::cout << std::endl << "(x) to end program" << std::endl << std::endl;
    }
    else
    {
        std::cout << "Error while opening folder:" << mPath << std::endl;
    }

    // Handle input
    std::string input;
    bool validInput = false;
    int selectedFileId;
    do
    {
        std::cin >> input;
        if (input == "x")
        {
            return 0;
        }
        else
        {
            std::string::const_iterator it = input.begin();
            while (it != input.end() && std::isdigit(*it)) ++it;
            if (!input.empty() && it == input.end())
            {
                selectedFileId = std::stoi(input);
                mSelectedFile = &filenames[selectedFileId];
                validInput = true;
            }
            else
            {
                std::cout << "Please select a file!" << std::endl;
            }
        }
    } while (!validInput);
    
    std::cout << "Selected File: " << *mSelectedFile << std::endl;

    InitBoard();
    Timing::getInstance()->startComputation();
    for (int i = 0; i < 250; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            int livingNeighbors = 0;
            for (int i = 0; i < 8; i++)
            {
                livingNeighbors += mBoard[j].mNeighbors[j]->mValue;
            }
            mTempBoard[j].mValue = livingNeighbors == 3 || (livingNeighbors == 2 && mBoard[j].mValue);
        }
        std::swap(mTempBoard, mBoard);
    }
    Timing::getInstance()->stopComputation();
    PrintBoard();
    Timing::getInstance()->print(true);
    return 0;
} 

bool ValidateArguments(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "FolderPath required" << std::endl;
        return false;
    }
    mPath = std::string(argv[1]) + "/in";
    return true;
}

bool InitBoard()
{
    Timing::getInstance()->startSetup();
    std::ifstream boardFile(mPath + "/" + *mSelectedFile);
    
    std::string line;
    char comma;
    boardFile >> mWidth;
    boardFile >> comma;
    boardFile >> mHeight;

    mSize = mWidth * mHeight;

    mBoard = new BoardCell[mSize];
    mTempBoard = new BoardCell[mSize];
    int lineCounter = 0;
    std::getline(boardFile, line);
    while (std::getline(boardFile, line))
    {
        for (uint32_t i = 0; i < line.size(); i++)
        {
            BoardCell bc;
            BoardCell tbc;
            bc.mValue = line[i] == 'x';

            int y_ = (lineCounter - 1 + mHeight) % mHeight;
            int x_ = (i - 1 + mWidth) % mWidth;
            int y1 = (lineCounter + 1 + mHeight) % mHeight;
            int x1 = (i + 1 + mWidth) % mWidth;

            bc.mNeighbors[0] = &mBoard[x_ + mWidth * y_];
            bc.mNeighbors[1] = &mBoard[i + mWidth * y_];
            bc.mNeighbors[2] = &mBoard[x1 + mWidth * y_];
            bc.mNeighbors[3] = &mBoard[x_ + mWidth * lineCounter];
            bc.mNeighbors[4] = &mBoard[x1 + mWidth * lineCounter];
            bc.mNeighbors[5] = &mBoard[x_ + mWidth * y1];
            bc.mNeighbors[6] = &mBoard[i + mWidth * y1];
            bc.mNeighbors[7] = &mBoard[x1 + mWidth * y1];

            tbc.mNeighbors[0] = &mTempBoard[x_ + mWidth * y_];
            tbc.mNeighbors[1] = &mTempBoard[i + mWidth * y_];
            tbc.mNeighbors[2] = &mTempBoard[x1 + mWidth * y_];
            tbc.mNeighbors[3] = &mTempBoard[x_ + mWidth * lineCounter];
            tbc.mNeighbors[4] = &mTempBoard[x1 + mWidth * lineCounter];
            tbc.mNeighbors[5] = &mTempBoard[x_ + mWidth * y1];
            tbc.mNeighbors[6] = &mTempBoard[i + mWidth * y1];
            tbc.mNeighbors[7] = &mTempBoard[x1 + mWidth * y1];

            mBoard[i + mWidth * lineCounter] = bc;
            mTempBoard[i + mWidth * lineCounter] = tbc;
        }
        ++lineCounter;
    }

    boardFile.close();
    Timing::getInstance()->stopSetup();
    return true;
}

bool PrintBoard()
{
    Timing::getInstance()->startFinalization();
    std::string outputPath = mPath + "/../out/" + *mSelectedFile + ".out";
    if (std::filesystem::exists(outputPath))
    {
        std::remove(outputPath.c_str());
    }
    std::ofstream boardFile(outputPath);

    boardFile << mWidth << "," << mHeight << std::endl;
    for (int y = 0; y < mHeight; y++)
    {
        for (int x = 0; x < mWidth; x++)
        {
            boardFile << (mBoard[x + mWidth * y].mValue ? 'x' : '.');
        }
        boardFile << std::endl;
    }
    boardFile.flush();

    boardFile.close();
    Timing::getInstance()->stopFinalization();
    return true;
}

bool GetFilenames(const std::string& path, std::vector<std::string>& filenames)
{
    if (!std::filesystem::exists(path))
    {
        return false;
    }
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        filenames.push_back(entry.path().string().erase(0, path.size() + 1));
    }
    return true;
}
