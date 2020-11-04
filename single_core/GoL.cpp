#include <fstream>
#include <filesystem>
#include <iostream>

#include "GoL.h"
#include "SingleCore.h"
#include "Timing.h"

int main(int argc, char** argv)
{
    // Handle arguments
    if (!LoadArguments(argc, argv))
    {
        return -1;
    }

    SC::PlayGame(mInputFile, mOutputFile, mNumGenerations);

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

