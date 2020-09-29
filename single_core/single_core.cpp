#include "single_core.h"

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
        for (int i = 0; i < filenames.size(); i++)
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
    for (int i = 0; i < 250; i++)
    {
        mBoard.PlayRound();
    }
    PrintBoard();
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
    std::ifstream boardFile(mPath + "/" + *mSelectedFile);
    
    std::string line;
    int width, height;
    char comma;
    boardFile >> width;
    boardFile >> comma;
    boardFile >> height;

    mBoard = Board(width, height);
    int lineCounter = 0;
    std::getline(boardFile, line);
    while (std::getline(boardFile, line))
    {
        mBoard.WriteLine(lineCounter++, line);
    }

    boardFile.close();
    return true;
}

bool PrintBoard()
{
    std::string outputPath = mPath + "/../out/" + *mSelectedFile + ".out";
    if (std::filesystem::exists(outputPath))
    {
        std::remove(outputPath.c_str());
    }
    std::ofstream boardFile(outputPath);

    const BoardCell* board = mBoard.GetBoard();
    boardFile << mBoard.mWidth << "," << mBoard.mHeight << std::endl;
    for (int y = 0; y < mBoard.mHeight; y++)
    {
        for (int x = 0; x < mBoard.mWidth; x++)
        {
            boardFile << (board[x + mBoard.mWidth * y].mValue ? 'x' : '.');
        }
        boardFile << std::endl;
    }
    boardFile.flush();

    boardFile.close();
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
