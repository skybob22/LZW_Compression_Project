#ifndef DECODE_DICTIONARY_H
#define DECODE_DICTIONARY_H

#include "HashTable.h"
#include <vector>


namespace Dictionary{
    template<typename T> //Make sure to use only unsigned numeric types
    class DecodeDictionary{
    private:
        //Will generate compile-time error if type is not unsigned
        static_assert(std::is_unsigned<T>::value,"Must use unsigned type");

        Hash::HashTable<T,std::vector<unsigned char>>* hashTable;
        T currentAssignValue;

        static unsigned long hashFunction(const T& key);
        static constexpr T endOfFile = static_cast<T>(0)-1; //Should wrap around to produce EOF/Out-of-space value

        static constexpr unsigned long DEFAULT_TABLE_SIZE = 100;

    public:

        DecodeDictionary(void);
        DecodeDictionary(unsigned long initialSize);
        ~DecodeDictionary(void);


        void add(const std::vector<unsigned char>& vector);
        void add(T prevCode,const unsigned char toAppend);
        T getLastAssignedKey() const;

        bool inDictionary(T key);
        std::vector<unsigned char> getBytes(T key);

        bool dictionaryFull(void) const;
    };

}

//====================Implementation====================//
namespace Dictionary {
    template<typename T>
    DecodeDictionary<T>::DecodeDictionary():DecodeDictionary(DEFAULT_TABLE_SIZE){
    }

    template <typename T>
    DecodeDictionary<T>::DecodeDictionary(unsigned long initialSize):currentAssignValue(0){
        hashTable = new Hash::HashTable<T,std::vector<unsigned char>>(hashFunction,initialSize);
        for(int i=0;i<256&&!dictionaryFull();i++){
            hashTable->insert(currentAssignValue,std::vector<unsigned char>({static_cast<unsigned char>(i)}));
            currentAssignValue++;
        }
    }

    template <typename T>
    DecodeDictionary<T>::~DecodeDictionary(){
        delete hashTable;
    }

    template <typename T>
    void DecodeDictionary<T>::add(const std::vector<unsigned char>& vector){
        if(currentAssignValue >= endOfFile-1){
            throw std::overflow_error("Dictionary is full");
        }
        else{
            hashTable->insert(currentAssignValue,vector);
            currentAssignValue++;
        }
    }

    template <typename T>
    void DecodeDictionary<T>::add(T prevCode,const unsigned char toAppend){
        if(!hashTable->inTable(prevCode)){
            throw std::runtime_error("Previous code does not exist in dictionary(D)");
        }
        else {
            std::vector<unsigned char> list = hashTable->getData(prevCode);
            list.push_back(toAppend);
            this->add(list);
        }
    }

    template <typename T>
    T DecodeDictionary<T>::getLastAssignedKey() const{
        return(currentAssignValue-static_cast<unsigned short>(1));
    }

    template <typename T>
    bool DecodeDictionary<T>::inDictionary(T key){
        return(hashTable->inTable(key));
    }

    template <typename T>
    std::vector<unsigned char> DecodeDictionary<T>::getBytes(T key){
        if(!hashTable->inTable(key)){
            throw std::runtime_error("Key is not in dictionary(D)");
        }
        else{
            return(hashTable->getData(key));
        }
    }

    template<typename T>
    bool DecodeDictionary<T>::dictionaryFull() const{
        return(currentAssignValue >= endOfFile-1);
    }

    template<typename T>
    unsigned long DecodeDictionary<T>::hashFunction(const T& key){
        std::hash<T> toLongHash;
        return(toLongHash(key));
    }
}
#endif