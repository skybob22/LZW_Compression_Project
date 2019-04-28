#include "gtest/gtest.h"
#include <fstream>
#include "LZWCodec.h"

namespace LZW {
    namespace LZWTest{
        long compareFileSize(const std::string& file1, const std::string& file2);
        bool fileEqual(const std::string& file1, const std::string& file2);
    }

    TEST(LZWCodec,VectorCompressionTest){
        std::string inputString = "The Fourier transform (FT) decomposes (also called analysis) a function of time (a signal) into its constituent frequencies";
        std::vector<unsigned char> inputVector(inputString.begin(),inputString.end());
        std::vector<unsigned short> encoded = LZWCodec::encode<unsigned short>(inputVector);
        EXPECT_LT(encoded.size(),inputVector.size());
        std::vector<unsigned char> outputVector = LZWCodec::decode<unsigned short>(encoded);
        EXPECT_EQ(outputVector,inputVector);
        std::string outputString(outputVector.begin(),outputVector.end());
        EXPECT_EQ(outputString,inputString);
    }

    TEST(LZWCodec,FileCompressionTestSmall){
        //Scope limiter for compression
        std::string inputFileName = "../../Media_Files/Tortoise_and_Hare.txt";
        std::string compressFileName = "../../Media_Files/Tortoise_and_Hare.lzwzip";
        std::string outputFileName = "../../Media_Files/Tortoise_and_Hare_Uncompressed.txt";
        {
            std::ifstream input(inputFileName, std::ios::binary);
            std::ofstream output(compressFileName, std::ios::binary);

            ASSERT_FALSE(input.fail());
            ASSERT_FALSE(output.fail());
            //If either fails to open, we cannot proceed
            ASSERT_NO_THROW(LZWCodec::encode(input, output));
            EXPECT_GE(LZWTest::compareFileSize(inputFileName,compressFileName),0);
            input.close();
            output.close();
        }
        //Scope limiter for decompression
        {
            std::ifstream input(compressFileName, std::ios::binary);
            std::ofstream output(outputFileName, std::ios::binary);
            ASSERT_FALSE(input.fail());
            ASSERT_FALSE(output.fail());
            //If either fails to open, we cannot proceed
            ASSERT_NO_THROW(LZWCodec::decode(input, output));
            input.close();
            output.close();
            //Expect size comparison
            EXPECT_TRUE(LZWTest::fileEqual(inputFileName,outputFileName));
        }
    }

    TEST(LZWCodec,FileCompressionTestLarge){
        //Scope limiter for compression
        std::string inputFileName = "../../Media_Files/The_Bible.txt";
        std::string compressFileName = "../../Media_Files/The_Bible.lzwzip";
        std::string outputFileName = "../../Media_Files/The_Bible_Uncompressed.txt";
        {
            std::ifstream input(inputFileName, std::ios::binary);
            std::ofstream output(compressFileName, std::ios::binary);

            ASSERT_FALSE(input.fail());
            ASSERT_FALSE(output.fail());
            //If either fails to open, we cannot proceed
            ASSERT_NO_THROW(LZWCodec::encode(input, output));

            input.close();
            EXPECT_GE(LZWTest::compareFileSize(inputFileName,compressFileName),0); //Expect the input to be larger than the output
            output.close();
        }
        //Scope limiter for decompression
        {
            std::ifstream input(compressFileName, std::ios::binary);
            std::ofstream output(outputFileName, std::ios::binary);
            ASSERT_FALSE(input.fail());
            ASSERT_FALSE(output.fail());
            //If either fails to open, we cannot proceed

            ASSERT_NO_THROW(LZWCodec::decode(input, output));
            input.close();
            output.close();

            EXPECT_EQ(LZWTest::compareFileSize(inputFileName,outputFileName),0);
            EXPECT_TRUE(LZWTest::fileEqual(inputFileName,outputFileName));
        }
    }

    TEST(LZWCodec,OtherFiletypesTest){
        //Scope limiter for compression
        std::string inputFileName = "../../Media_Files/Night_Drive.mp3";
        std::string compressFileName = "../../Media_Files/Night_Drive.lzwzip";
        std::string outputFileName = "../../Media_Files/Night_Drive_Uncompressed.mp3";
        {
            std::ifstream input(inputFileName, std::ios::binary);
            std::ofstream output(compressFileName, std::ios::binary);

            ASSERT_FALSE(input.fail());
            ASSERT_FALSE(output.fail());
            //If either fails to open, we cannot proceed
            ASSERT_NO_THROW(LZWCodec::encode(input, output));

            input.close();
            output.close();
        }
        //Scope limiter for decompression
        {
            std::ifstream input(compressFileName, std::ios::binary);
            std::ofstream output(outputFileName, std::ios::binary);
            ASSERT_FALSE(input.fail());
            ASSERT_FALSE(output.fail());
            //If either fails to open, we cannot proceed

            ASSERT_NO_THROW(LZWCodec::decode(input, output));
            input.close();
            output.close();

            EXPECT_EQ(LZWTest::compareFileSize(inputFileName,outputFileName),0);
            EXPECT_TRUE(LZWTest::fileEqual(inputFileName,outputFileName));
        }
    }




    namespace LZWTest{
        long compareFileSize(const std::string& file1, const std::string& file2){
            std::ifstream inputFile1(file1);
            std::ifstream inputFile2(file2);

            if(inputFile1.fail()){
                throw(std::runtime_error("Failure opening file 1"));
            }
            if(inputFile2.fail()){
                throw(std::runtime_error("Failure opening file 2"));
            }

            //Set the position of the next character for each file to the end of file
            inputFile1.seekg(0,inputFile1.end);
            inputFile2.seekg(0,inputFile2.end);

            //Will return positive, negative or 0 depending on which position is bigger (larger file)
            return(inputFile1.tellg()-inputFile2.tellg());
        }

        bool fileEqual(const std::string& file1, const std::string& file2){
            std::ifstream inputFile1(file1);
            std::ifstream inputFile2(file2);

            if(inputFile1.fail()){
                throw(std::runtime_error("Failure opening file 1"));
            }
            if(inputFile2.fail()){
                throw(std::runtime_error("Failure opening file 2"));
            }

            //Set the position of the next character for each file to the end of file
            inputFile1.seekg(inputFile1.end);
            inputFile2.seekg(inputFile2.end);

            if(inputFile1.tellg() != inputFile2.tellg()){
                //If the files are not the same size, they cannot be the same
                return(false);
            }
            else{
                //Set the position of the next character for each file to the beginning of file
                inputFile1.seekg(inputFile1.beg);
                inputFile2.seekg(inputFile2.beg);

                while(!inputFile1.eof() && !inputFile2.eof()){
                    if(inputFile1.get() != inputFile2.get()){
                        return(false);
                    }
                    if(inputFile1.eof() || inputFile2.eof()){
                        break;
                    }
                }
                if(inputFile1.eof() != inputFile2.eof()){
                    return(false);
                }
                else {
                    return (true);
                }
            }

        }
    }
}