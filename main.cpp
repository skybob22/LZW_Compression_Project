#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "LZWCodec.h"

int main() {
    for(bool exit = false;!exit;exit=exit){
        std::string userCommand;
        std::cout   << "Enter Command" << std::endl
                    << "1: Compress" << std::endl
                    << "2: Decompress" << std::endl
                    << "3: Exit" << std::endl
                    << "Command:"
                    << std::flush;

        std::cin >> userCommand;
        std::transform(userCommand.begin(),userCommand.end(),userCommand.begin(),tolower);

        if(userCommand == "compress" || userCommand == "1"){
            std::cout << std::endl << "Enter target filepath/name:" << std::flush;
            std::string inputFileName,outputFileName;
            std::cin >> inputFileName;

            std::ifstream inputFile(inputFileName, std::ios::binary);
            if(inputFile.fail()){
                std::cout << "Unable to open target file" << std::endl << std::endl << std::flush;
                continue;
            }

            std::cout << "Enter destination filepath/name:" << std::flush;
            std::cin >> outputFileName;

            std::ofstream outputFile(outputFileName,std::ios::binary);
            if(outputFile.fail()){
                std::cout << "Unable to open destination file" << std::endl << std::endl << std::flush;
                continue;
            }

            LZW::LZWCodec::encode(inputFile,outputFile);
            inputFile.close();
            outputFile.close();
            std::cout << "Compression Finished" << std::endl;
        }
        else if(userCommand == "decompress" || userCommand == "2"){
            std::cout << std::endl << "Enter target filepath/name:" << std::flush;
            std::string inputFileName,outputFileName;
            std::cin >> inputFileName;

            std::ifstream inputFile(inputFileName, std::ios::binary);
            if(inputFile.fail()){
                std::cout << "Unable to open target file" << std::endl << std::endl << std::flush;
                continue;
            }

            std::cout << "Enter destination filepath/name:" << std::flush;
            std::cin >> outputFileName;

            std::ofstream outputFile(outputFileName,std::ios::binary);
            if(outputFile.fail()){
                std::cout << "Unable to open destination file" << std::endl << std::endl << std::flush;
                continue;
            }

            LZW::LZWCodec::decode(inputFile,outputFile);
            inputFile.close();
            outputFile.close();
            std::cout << "Decompression Finished" << std::endl;

        }
        else if(userCommand == "exit" || userCommand == "3"){
            exit = true;
        }
        else{
            std::cout << "Invalid command. Please enter a vaild command" << std::endl << std::flush;
        }

        if(!exit){
            std::cout << std::endl;
        }
    }
    return 0;
}