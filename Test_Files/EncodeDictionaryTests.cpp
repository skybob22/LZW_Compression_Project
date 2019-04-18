#include "gtest/gtest.h"
#include "EncodeDictionary.h"

namespace DictionaryTests{
    using Dictionary::EncodeDictionary;

    TEST(Encode_Dictionary, constructorTest) {
        ASSERT_NO_FATAL_FAILURE(EncodeDictionary<unsigned short> dict_1);
        ASSERT_NO_FATAL_FAILURE(EncodeDictionary<unsigned short> dict_2(20));
        EncodeDictionary<unsigned short> dict_3;
        for (int i = 0; i < 256; i++) {
            std::vector<unsigned char> base = {static_cast<unsigned char>(i)};
            EXPECT_EQ(static_cast<unsigned short>(i), dict_3.getCode(base));
        }
    }

    TEST(Encode_Dictionary,destructorTest) {
        EncodeDictionary<unsigned short>* dict_1 = new EncodeDictionary<unsigned short>;
        for (unsigned char i=0;i<255;i++) {
            ASSERT_NO_THROW(dict_1->add(std::vector<unsigned char>({i})));
            EXPECT_EQ(i,dict_1->add(std::vector<unsigned char>({i})));
        }
        ASSERT_NO_FATAL_FAILURE(delete dict_1);
    }

    TEST(Encode_Dictionary,singleByteTest) {
        EncodeDictionary<unsigned short> dict_1;

        for (unsigned int i=0;i<256;i++){
            EXPECT_TRUE(dict_1.inDictionary(i));
            EXPECT_EQ(static_cast<unsigned short>(i),dict_1.getCode(static_cast<unsigned char>(i)));
        }
    }

    TEST(Encode_Dictionary,duplicateAddTest){
        EncodeDictionary<unsigned short> dict_1;
        for (unsigned char i=0;i<255;i++) {    //Fills table with entries to delete
            ASSERT_NO_THROW(dict_1.add(std::vector<unsigned char>({i})));
            EXPECT_EQ(i,dict_1.add(std::vector<unsigned char>({i})));
        }
    }

    TEST(Encode_Dictionary,notInDictTest){
        EncodeDictionary<unsigned short> dict_1;
        std::vector<unsigned char> temp = {5,1};

        EXPECT_FALSE(dict_1.inDictionary(temp));
        ASSERT_ANY_THROW(dict_1.getCode(temp));
    }

    TEST(Encode_Dictionary,vectorTest) {
        std::vector<unsigned char> test_key_1 = {0,1,2,3,4,5};
        std::vector<unsigned char> test_key_2 = {3,1,4,1};
        std::vector<unsigned char> test_key_3 = {1,1};
        std::vector<unsigned char> test_key_4 = {0,0};
        std::vector<unsigned char> test_key_5 = {1,1,1,1,1,1};

        EncodeDictionary<unsigned short> dict_1;
        ASSERT_NO_THROW(dict_1.add(test_key_1));
        ASSERT_NO_THROW(dict_1.add(test_key_2));
        ASSERT_NO_THROW(dict_1.add(test_key_3));

        EXPECT_TRUE(dict_1.inDictionary(test_key_1));
        EXPECT_TRUE(dict_1.inDictionary(test_key_2));
        EXPECT_TRUE(dict_1.inDictionary(test_key_3));
        EXPECT_FALSE(dict_1.inDictionary(test_key_4));

        ASSERT_NO_THROW(dict_1.getCode(test_key_1));
        EXPECT_EQ(256,dict_1.getCode(test_key_1));
        ASSERT_NO_THROW(dict_1.getCode(test_key_2));
        EXPECT_EQ(257,dict_1.getCode(test_key_2));
        ASSERT_NO_THROW(dict_1.getCode(test_key_3));
        EXPECT_EQ(258,dict_1.getCode(test_key_3));
        ASSERT_ANY_THROW(dict_1.getCode(test_key_4));
        EXPECT_EQ(259,dict_1.add(test_key_5));
    }

    TEST(Encode_Dictionary,dictionaryFullTest){
        EncodeDictionary<unsigned char> dict_1;
        //Unsigned char can't even hold the basic elements, it doesn't have room for more
        EXPECT_ANY_THROW(dict_1.add(std::vector<unsigned char>{1,0}));
    }

    TEST(Encode_Dictionary,differentTypesTest){
        //The code should be able to compile and execute with any unsigned type
        EncodeDictionary<unsigned char> dict_1;
        EncodeDictionary<unsigned short> dict_2;
        EncodeDictionary<unsigned int> dict_3;
        EncodeDictionary<unsigned long> dict_4;
        EXPECT_TRUE(true); //Just serves to tell that code execution reached this far
    }
}