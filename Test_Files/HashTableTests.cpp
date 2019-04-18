#include "gtest/gtest.h"
#include "HashTable.h"
#include <string>

namespace HashTests{
    using Hash::HashTable;

    unsigned long intKey(const int& a){
        unsigned long x = a;
        x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
        x = x ^ (x >> 31);
        return x;
    }

    unsigned long stringKey(const std::string& a){
        std::hash<std::string> fcnPtr;
        return(static_cast<unsigned long>(fcnPtr(a)));
    }

    TEST(Hash_Table,ConstructorTest){
        HashTable<int,int>* testFixture; // = new Hash::HashTable<int,int>(fncPtr);
        testFixture = new Hash::HashTable<int,int>(intKey);
        ASSERT_NO_FATAL_FAILURE(delete testFixture);
    }

    TEST(Hash_Table,InsertionTest){
        HashTable<std::string,int> table(stringKey);

        std::string input[] = {"Cake","Pie","Pizza","Ice Cream","Calzone","Funnel Cake","Clam Chowder"};
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            ASSERT_NO_FATAL_FAILURE(table.insert(input[i],i));
        }

        HashTable<std::string,int> table2(stringKey);
        table2.insert("Test String",22);
        EXPECT_EQ(static_cast<double>(1)/100,table2.getLoadFactor()); //Table defaults to size of 100
    }

    TEST(Hash_Table,SearchTest){
        HashTable<std::string,int> table(stringKey);

        std::string input[] = {"Cake","Pie","Pizza","Ice Cream","Calzone","Funnel Cake","Clam Chowder"};
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            EXPECT_NO_THROW(table.insert(input[i],i*i));
        }
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            EXPECT_TRUE(table.inTable(input[i]));
            EXPECT_EQ(table.getData(input[i]),i*i);
        }
        EXPECT_FALSE(table.inTable("Pumpkin"));
        ASSERT_ANY_THROW(table.getData("Pumpkin"));
    }

    TEST(Hash_Table,RemoveTest){
        HashTable<std::string,int> table(stringKey);

        std::string input[] = {"Cake","Pie","Pizza","Ice Cream","Calzone","Funnel Cake","Clam Chowder"};
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            ASSERT_NO_THROW(table.insert(input[i],i));
        }
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            ASSERT_NO_THROW(table.remove(input[i]));
        }
    }

    TEST(Hash_Table,AutoRehashTest){
        auto table = new HashTable<std::string,int>(stringKey,1);
        table->setAutoRehash(false);

        std::string input[] = {"Cake","Pie","Pizza","Ice Cream","Calzone","Funnel Cake","Clam Chowder"};
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            EXPECT_NO_THROW(table->insert(input[i],i));
        }
        EXPECT_GT(table->getLoadFactor(),1);
        table->setAutoRehash(true);
        EXPECT_LT(table->getLoadFactor(),1);
        EXPECT_EQ(table->getData("Cake"),0);
        EXPECT_EQ(table->getData("Pizza"),2);
    }

    TEST(Hash_Table,LinkedListTest){
        //Forces the table to behave like a linked list for testing purposes
        auto table = new HashTable<std::string,int>(stringKey,1);
        table->setAutoRehash(false);

        std::string input[] = {"Cake","Pie","Pizza","Ice Cream","Calzone","Funnel Cake","Clam Chowder"};
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            ASSERT_NO_THROW(table->insert(input[i],i));
        }
        EXPECT_GT(table->getLoadFactor(),1);
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            EXPECT_EQ(table->getData(input[i]),i);
        }
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            EXPECT_NO_THROW(table->remove(input[i]));
        }
        ASSERT_NO_FATAL_FAILURE(delete table);
    }

    TEST(Hash_Table,OverWriteTest){
        auto table = new HashTable<std::string,int>(stringKey,1);

        std::string input[] = {"Cake","Pie","Pizza","Ice Cream","Calzone","Funnel Cake","Clam Chowder"};
        for(int i=0;i<sizeof(input)/sizeof(input[0]);i++){
            EXPECT_NO_THROW(table->insert(input[i],i));
        }
        ASSERT_NO_THROW(table->overWrite("Pie",314));
        EXPECT_EQ(table->getData("Pie"),314);
        ASSERT_ANY_THROW(table->overWrite("Spagett",10));
    }

    TEST(Hash_Table,dataIntegrityTest){
        auto table = new HashTable<int,std::string>(intKey,1);
        table->setAutoRehash(false);

        std::string test1,test2;
        test1 = test2 = "This is a test";
        table->insert(10,test1);
        test1 = "I have changed the data";
        EXPECT_EQ(table->getData(10),test2);
        table->overWrite(10,test1);
        test2 = test1;
        EXPECT_EQ(table->getData(10),test1);
        test1 = "One more time";
        EXPECT_EQ(table->getData(10),test2);
        delete table;
    }

}