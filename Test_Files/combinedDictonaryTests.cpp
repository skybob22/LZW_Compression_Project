#include "gtest/gtest.h"
#include "EncodeDictionary.h"
#include "DecodeDictionary.h"

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
        EncodeDictionary<unsigned short> encDict;
        std::vector<unsigned short> encodedOutput;

        std::vector<unsigned char> currentPattern;
        for(unsigned char current : input)
        {
            if(encDict.inDictionary(currentPattern+current))
            {
                currentPattern.push_back(current);
            }
            else
            {
                encodedOutput.push_back(encDict.getCode(currentPattern));
                currentPattern.push_back(current);
                encDict.add(currentPattern);
                currentPattern = {current};
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
            EXPECT_TRUE(decDict.inDictionary(currentCode));
            if(!decDict.inDictionary(currentCode))
            {
                throw(std::runtime_error("code doesn't exist"));
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
}