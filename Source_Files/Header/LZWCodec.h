#ifndef LZW_COMPRESSION_LZWCODEC_H
#define LZW_COMPRESSION_LZWCODEC_H

#include <vector>
#include <iostream>
#include "DecodeDictionary.h"
#include "EncodeDictionary.h"

namespace LZW
{
    class LZWCodec
    {
    private:
        static constexpr unsigned short MAXIMUM_UNSIGNED_SHORT = static_cast<unsigned short>(0)-1;
        static constexpr unsigned int MAXIMUM_UNSIGNED_INT = static_cast<unsigned int>(0)-1;
        static constexpr unsigned long MAXIMUM_UNSIGNED_LONG = static_cast<unsigned long>(0)-1;
        static constexpr unsigned long long MAXIMUM_UNSIGNED_LONG_LONG = static_cast<unsigned long long>(0)-1;

        template<typename T>
        static void compressToStream(std::istream& input, std::ostream& output, long inputSize);
        template<typename T>
        static void decompressToStream(std::istream& input, std::ostream& output, long inputSize);

    public:
        template<typename T>
        static std::vector<T> encode(const std::vector<unsigned char>& input);
        static void encode(std::istream& input, std::ostream& output);

        template<typename T>
        static std::vector<unsigned char> decode(const std::vector<T>& input);
        static void decode(std::istream& input, std::ostream& output);
    };
}
//====================Implementation====================//
namespace LZW
{
    //===============Utility Functions===============//
    template<typename T>
    std::vector<T> operator+(std::vector<T> inputVector,T toAdd);
    std::ostream& operator<<(std::ostream &stream, const std::vector<unsigned char> &vect);
    //===============================================//
}
namespace LZW
{
    using Dictionary::EncodeDictionary;
    using Dictionary::DecodeDictionary;

    template<typename T>
    std::vector<T> LZWCodec::encode(const std::vector<unsigned char> &input)
    {
        //Will generate compile-time error if type is not unsigned
        static_assert(std::is_unsigned<T>::value,"Must use unsigned type");

        if (input.empty())
        {
            //If the input is empty, there is no work to be done. Return empty vector
            return (std::vector<T>());
        }

        EncodeDictionary<T> encDict(std::max(static_cast<unsigned long>(input.size()*1.3),static_cast<unsigned long>(1024)));
        //Begin compressing data
        std::vector<T> encodedOutput;
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

        return (encodedOutput);
    }

    template<typename T>
    std::vector<unsigned char> LZWCodec::decode(const std::vector<T> &input)
    {
        //Will generate compile-time error if type is not unsigned
        static_assert(std::is_unsigned<T>::value,"Must use unsigned type");

        if (input.empty())
        {
            //If the input is empty, there is no work to be done. Return empty vector
            return (std::vector<unsigned char>());
        }

        DecodeDictionary<T> decDict(std::max(static_cast<unsigned long>(input.size()*1.3),static_cast<unsigned long>(1024)));

        //Begin decompressing data
        std::vector<unsigned char> decodedOutput;

        std::vector<unsigned char> currentPattern;
        T previousCode = input[0];
        T currentCode = 0;

        //Start here because the first byte can never be a non-single byte
        currentPattern = decDict.getBytes(previousCode);
        decodedOutput.insert(decodedOutput.end(), currentPattern.begin(), currentPattern.end());

        for (unsigned long currentPos = 1; currentPos < input.size(); currentPos++)
        {
            currentCode = input[currentPos];
            if (!decDict.inDictionary(currentCode))
            {
                //throw(std::runtime_error("Code doesn't exist")); //Since the dictionary is built as it goes, it should never encounter a code not in the dictionary
                currentPattern.push_back(currentPattern[0]);
            } else
                {
                currentPattern = decDict.getBytes(currentCode);
            }
            decodedOutput.insert(decodedOutput.end(), currentPattern.begin(), currentPattern.end());
            decDict.add(previousCode, currentPattern[0]);
            previousCode = currentCode;
        }

        return (decodedOutput);
    }


    template<typename T>
    void LZWCodec::compressToStream(std::istream& input, std::ostream& output,long inputSize)
    {
        EncodeDictionary<T> encDict(std::max(static_cast<long>(inputSize*1.3), static_cast<long>(1024)));

        std::vector<unsigned char> currentPattern;
        unsigned char currentChar;
        while (!input.eof())
        {
            currentChar = input.get();
            if (input.eof())
            {
                //If reading fails due to end of file
                break;
            }

            if (encDict.inDictionary(currentPattern + currentChar))
            {
                currentPattern.push_back(currentChar);
            } else
                {
                T temp = encDict.getCode(currentPattern);
                output.write(reinterpret_cast<const char *>(&temp), sizeof(T));
                currentPattern.push_back(currentChar);
                encDict.add(currentPattern);
                currentPattern = {currentChar};
            }
        }
        T temp = encDict.getCode(currentPattern);
        output.write(reinterpret_cast<const char *>(&temp), sizeof(T));
    }

    template<typename T>
    void LZWCodec::decompressToStream(std::istream& input, std::ostream& output,long inputSize)
    {
        DecodeDictionary<T> decDict(std::max(static_cast<long>(inputSize*1.3),static_cast<long>(1024)));
        std::vector<unsigned char> currentPattern;
        T currentCode, previousCode;

        input.read(reinterpret_cast<char *>(&previousCode), sizeof(T));
        currentPattern = decDict.getBytes(static_cast<T>(previousCode));
        output << currentPattern;

        while (!input.eof() && !decDict.dictionaryFull())
        {
            input.read(reinterpret_cast<char *>(&currentCode), sizeof(T));
            if (input.eof() || decDict.dictionaryFull())
            {
                //If we have reached the end of the file or the dictionary is full, quit
                break;
            }
            if (!decDict.inDictionary(static_cast<unsigned int>(currentCode)))
            {
                currentPattern.push_back(currentPattern[0]);
            }
            else
                {
                currentPattern = decDict.getBytes(static_cast<T>(currentCode));
            }
            output << currentPattern;
            decDict.add(previousCode, currentPattern[0]);
            previousCode = currentCode;
        }
    }
}
namespace LZW
{
    //===============Utility Functions===============//
    template<typename T>
    std::vector <T> operator+(std::vector <T> inputVector, T toAdd)
            {
        inputVector.push_back(toAdd);
        return (inputVector);
    }
    //===============================================//
}
#endif //LZW_COMPRESSION_LZWCODEC_H
