#include "gtest/gtest.h"
#include "DecodeDictionary.h"

namespace DictionaryTests{
    using Dictionary::DecodeDictionary;

    TEST(Decode_Dictionary,constructorTest){
        EXPECT_NO_FATAL_FAILURE(DecodeDictionary<unsigned short> dict_1);
        EXPECT_NO_FATAL_FAILURE(DecodeDictionary<unsigned short> dict_2(20));
        DecodeDictionary<unsigned short> dict_3;
        for (int i=0;i<256;i++) {
            EXPECT_EQ(std::vector<unsigned char>({static_cast<unsigned char>(i)}),dict_3.getBytes(i));
        }
    }

    TEST(Decode_Dictionary,destructorTest){
        DecodeDictionary<unsigned short>* dict_1 = new DecodeDictionary<unsigned short>;
        EXPECT_NO_FATAL_FAILURE(delete dict_1);
    }

    TEST(Decode_Dictionary,addTest) {
        DecodeDictionary<unsigned short> dict_1;

        std::vector<unsigned char> temp = {1,2};
        EXPECT_NO_THROW(dict_1.add(temp));
        EXPECT_EQ(temp,dict_1.getBytes(256));
        EXPECT_NO_THROW(dict_1.add(0,static_cast<unsigned char>(10)));
        temp = {0,10};
        EXPECT_EQ(temp,dict_1.getBytes(257));

        EXPECT_NO_FATAL_FAILURE(dict_1.add(257,static_cast<unsigned char>(99)));
        temp = {0,10,99};
        EXPECT_EQ(temp,dict_1.getBytes(258));
    }

    TEST(Decode_Dictionary,notInDictTest){
        DecodeDictionary<unsigned short> dict_1;

        EXPECT_FALSE(dict_1.inDictionary(256));
        EXPECT_ANY_THROW(dict_1.getBytes(256));
    }

    TEST(Decode_Dictionary,dictionaryFullTest){
        DecodeDictionary<unsigned char> dict_1;
        //Unsigned char can't even hold the basic elements, it doesn't have room for more
        EXPECT_ANY_THROW(dict_1.add(dict_1.getLastAssignedKey(),1));

        DecodeDictionary<unsigned short> dict_2;
        EXPECT_NO_THROW(dict_2.add(dict_2.getLastAssignedKey(),0));
        std::vector<unsigned char> vec{255,1};
        for(int i=256;i<65535-2;i++){
            if(vec.back() >= 255){
                vec.push_back(1);
            }
            else{
                vec.back()++;
            }
            EXPECT_NO_THROW(dict_2.add(vec));
        }
        vec.push_back(1);
        EXPECT_ANY_THROW(dict_2.add(vec));
    }

}