#include <fstream>
#include <filesystem>
#include <iostream>
#include <thread>

#include "GoL.h"
#include "SingleCore.h"
#include "MultiCoreMP.h"
#include "Timing.h"

int main(int argc, char** argv)
{
    // Handle arguments
    if (!LoadArguments(argc, argv))
    {
        return -1;
    }

    switch (mMode)
    {
        case SEQ:
            SC::PlayGame(mInputFile, mOutputFile, mNumGenerations);
            break;

        case OMP:
            MP::PlayGame(mInputFile, mOutputFile, mNumGenerations, mOmpThreads);
            break;

        case OCL:
            break;

        default:
            break;
    }


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
    bool modeProvided = false;
    bool threadsProvided = false;
    bool deviceProvided = false;

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
                std::cout << "No generations provided!" << std::endl;
                noError = false;
            }
        }
        else if (arg == "--measure")
        {
            mPrintMeasurment = true;
        }
        else if (arg == "--mode")
        {
            if ((i + 1) < argc)
            {
                std::string mode(argv[i + 1]);
                modeProvided = true;
                if (mode == "seq")
                {
                    mMode = SEQ;
                }
                else if (mode == "omp")
                {
                    mMode = OMP;
                }
                else if (mode == "ocl")
                {
                    mMode = OCL;
                }
                else
                {
                    modeProvided = false;
                }
            }
            else
            {
                std::cout << "No Mode provided!" << std::endl;
                noError = false;
            }
        }
        else if (arg == "--threads")
        {
            if ((i + 1) < argc)
            {
                mOmpThreads = std::stoi(argv[i + 1]);
                threadsProvided = true;
            }
            else
            {
                std::cout << "No Threads provided!" << std::endl;
                noError = false;
            }
        }
        else if (arg == "--device")
        {
            if ((i + 1) < argc)
            {
                std::string device(argv[i + 1]);
                deviceProvided = true;
                if (device == "cpu")
                {
                    mDevice = CPU;
                }
                else if (device == "gpu")
                {
                    mDevice = GPU;
                }
                else
                {
                    deviceProvided = false;
                }
            }
            else
            {
                std::cout << "No Device provided!" << std::endl;
                noError = false;
            }
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
    if (!modeProvided)
    {
        mMode = SEQ;
        std::cout << "No mode provided or mode not recognized! Using default: " << "seq" << std::endl;
    }
    if (mMode == OMP)
    {
        if (!threadsProvided)
        {
            mOmpThreads = std::thread::hardware_concurrency();
            std::cout << "No number of threads provided! Using default: " << mOmpThreads << std::endl;
        }
    }
    if (mMode == OCL)
    {
        if (!deviceProvided)
        {
            mDevice = GPU;
            std::cout << "No device provided or device not recognized! Using default: " << "gpu" << std::endl;
        }
    }
    return noError;
}

