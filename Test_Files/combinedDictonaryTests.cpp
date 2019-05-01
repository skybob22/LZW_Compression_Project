#include "gtest/gtest.h"
#include "EncodeDictionary.h"
#include "DecodeDictionary.h"
#include <chrono>
#include <fstream>

namespace DictionaryTests
{
    using Dictionary::EncodeDictionary;
    using Dictionary::DecodeDictionary;

    std::vector<unsigned char> operator+(std::vector<unsigned char> vector, unsigned char toAdd)
    {
        vector.push_back(toAdd);
        return(vector);
    }

    TEST(Combined_Dictionary, EncodeDecodeTest)
    {
        std::string inputString = "This is a test to see how well the dictionaries are able to compress some random given input. We also expect it to be able to decompress the output back to the original";
        std::vector<unsigned char> input;
        for(int i = 0; i < inputString.size(); i++)
        {
            input.push_back(inputString[i]);
        }

        EncodeDictionary<unsigned short> encDict(std::max(static_cast<unsigned long>(input.size()*1.3),static_cast<unsigned long>(1024)));
        std::vector<unsigned short> encodedOutput;
        std::vector<unsigned char> currentPattern;

        for (unsigned char currentChar : input)
        {
            if (encDict.inDictionary(currentPattern + currentChar))
            {
                currentPattern.push_back(currentChar);
            } else {
                encodedOutput.push_back(encDict.getCode(currentPattern));
                currentPattern.push_back(currentChar);
                encDict.add(currentPattern);
                currentPattern = {currentChar};
            }
        }
        encodedOutput.push_back(encDict.getCode(currentPattern));
        EXPECT_LT(encodedOutput.size(),input.size());

        DecodeDictionary<unsigned short> decDict;
        unsigned short previousCode=encodedOutput[0];
        unsigned short currentCode = 0;
        std::vector<unsigned char> decodedOutput;

        currentPattern.clear();
        currentPattern = decDict.getBytes(previousCode);
        decodedOutput.insert(decodedOutput.end(), currentPattern.begin(), currentPattern.end());
        for(unsigned int currentPos = 1;currentPos<encodedOutput.size();currentPos++)
        {
            currentCode = encodedOutput[currentPos];
            if(!decDict.inDictionary(currentCode))
            {
                currentPattern.push_back(currentPattern[0]);
            }
            else
            {
                currentPattern = decDict.getBytes(currentCode);
            }
            decodedOutput.insert(decodedOutput.end(), currentPattern.begin(), currentPattern.end());
            decDict.add(previousCode, currentPattern[0]);
            previousCode = currentCode;
        }
        EXPECT_EQ(decodedOutput, input);
        std::string outputString;
        for(unsigned char i : decodedOutput)
        {
            outputString.push_back(i);
        }
        EXPECT_EQ(outputString,inputString);
    }

    void compressCycle(std::vector<unsigned char>& input,std::vector<unsigned char>& output,bool linkedListMode){
        EncodeDictionary<unsigned int> encDict(
                !linkedListMode?1:std::max(static_cast<unsigned long>(input.size() * 1.3), static_cast<unsigned long>(1024)),linkedListMode);
        std::vector<unsigned int> encodedOutput;
        std::vector<unsigned char> currentPattern;

        for (unsigned char currentChar : input) {
            if (encDict.inDictionary(currentPattern + currentChar)) {
                currentPattern.push_back(currentChar);
            } else {
                encodedOutput.push_back(encDict.getCode(currentPattern));
                currentPattern.push_back(currentChar);
                encDict.add(currentPattern);
                currentPattern = {currentChar};
            }
        }
        encodedOutput.push_back(encDict.getCode(currentPattern));

        DecodeDictionary<unsigned int> decDict(
                !linkedListMode?1:std::max(static_cast<unsigned long>(input.size() * 1.3), static_cast<unsigned long>(1024)),linkedListMode);
        unsigned int previousCode = encodedOutput[0];
        unsigned int currentCode = 0;
        output.clear();

        currentPattern.clear();
        currentPattern = decDict.getBytes(previousCode);
        output.insert(output.end(), currentPattern.begin(), currentPattern.end());
        for (unsigned int currentPos = 1; currentPos < encodedOutput.size(); currentPos++) {
            currentCode = encodedOutput[currentPos];
            if (!decDict.inDictionary(currentCode)) {
                currentPattern.push_back(currentPattern[0]);
            } else {
                currentPattern = decDict.getBytes(currentCode);
            }
            output.insert(output.end(), currentPattern.begin(), currentPattern.end());
            decDict.add(previousCode, currentPattern[0]);
            previousCode = currentCode;
        }
    }


    //Warning, this takes approximately 6 hours to run, don't run it unless you really  mean to
    //Need to remove "DISABLED_" before running, and put back afterwards
    TEST(DISABLED_Combined_Dictionary,TimeComplexityTest){
        const unsigned int number_of_iterations = 100000;
        std::ofstream outputFileHash("../../Test_Files/Runtime_Data/Hash_Runtime.dat");
        std::ofstream outputFileLink("../../Test_Files/Runtime_Data/Link_Runtime.dat");

        for (unsigned int i=1;i<number_of_iterations+1;i+=100) {
            for (int z = 0; z < 2; z++) { //Used for linked_list vs hash_table

                std::vector<unsigned char> input;
                std::vector<unsigned char> output;
                for (unsigned int j = 0; j < i; j++) {
                    input.push_back(std::rand() % (static_cast<unsigned char>(0) - 10));
                }

                //Start timing the algorithm
                auto start = std::chrono::system_clock::now();

                compressCycle(input,output,z==0);

                //Stop timing the algorithm
                auto end = std::chrono::system_clock::now();
                EXPECT_EQ(output, input);

                if(z == 0) {
                    outputFileHash << i << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                               << std::endl;
                }
                else if(z == 1){
                    outputFileLink << i << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                                   << std::endl;
                }
            }
        }
        outputFileHash.close();
    }
}