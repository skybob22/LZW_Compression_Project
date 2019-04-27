#include <cstring>
#include <fstream>
#include "LZWCodec.h"

namespace LZW
{
    void LZWCodec::encode(std::istream& input, std::ostream& output)
    {
        if(!input.good())
        {
            throw (std::runtime_error("Bad Input Stream"));

        }
        if (!output.good())
        {
            throw (std::runtime_error("Bad Output stream"));
        }
        if (input.peek() == std::istream::traits_type::eof())
        {
            return;
        }
        input.seekg(0,input.end);
        long inputSize = input.tellg();
        input.seekg(0, input.beg);

        if(inputSize < MAXIMUM_UNSIGNED_SHORT-255)
        {
            const char temp = 0;
            output.write(&temp, sizeof(temp));
            compressToStream<unsigned short>(input,output,inputSize);
        }
        else if (inputSize < MAXIMUM_UNSIGNED_INT-255)
        {
            const char temp = 1;
            output.write(&temp, sizeof(temp));
            compressToStream<unsigned int>(input,output,inputSize);

        }
        else if (inputSize < MAXIMUM_UNSIGNED_LONG-255)
        {
            const char temp = 2;
            output.write(&temp, sizeof(temp));
            compressToStream<unsigned long>(input,output,inputSize);
        }
        else if (inputSize < MAXIMUM_UNSIGNED_LONG_LONG-255)
        {
            const char temp = 3;
            output.write(&temp, sizeof(temp));
            compressToStream<unsigned long long>(input,output,inputSize);
        }
        else
        {
            throw(std::runtime_error("File is too large to properly compress"));
        }
        return;
    }
    void LZWCodec::decode(std::istream &input, std::ostream &output)
    {
        if (!input.good())
        {
            throw (std::runtime_error("Bad input stream"));
        }
        if (!output.good())
        {
            throw (std::runtime_error("Bad output stream"));
        }
        if (input.peek() == std::istream::traits_type::eof())
        {
            return;
        }
        input.seekg(0,input.end);
        long inputSize = input.tellg();
        input.seekg(0,input.beg);

        char typeCode;
        input.read(&typeCode,sizeof(char));
        switch(typeCode)
        {
            case 0:
                decompressToStream<unsigned short>(input,output,inputSize);
                break;
            case 1:
                decompressToStream<unsigned int>(input,output,inputSize);
                break;
            case 2:
                decompressToStream<unsigned long>(input,output,inputSize);
                break;
            case 3:
                decompressToStream<unsigned long long>(input,output,inputSize);
                break;
            default:
                throw(std::runtime_error("Corrupt type specifier"));
        }
        return;
    }
}