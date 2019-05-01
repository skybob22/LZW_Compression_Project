#ifndef ENCODE_DICTIONARY_H
#define ENCODE_DICTIONARY_H

#include "HashTable.h"
#include <vector>

namespace Dictionary{
    template <typename T> //Make sure to use only unsigned numeric types
    class EncodeDictionary{
    private:
        //Will generate compile-time error if type is not unsigned
        static_assert(std::is_unsigned<T>::value,"Must use unsigned type");

        Hash::HashTable<std::vector<unsigned char>,T>* hashTable;
        T currentAssignValue;

        static unsigned long hashFunction(const std::vector<unsigned char>& vector);
        static constexpr T endOfFile = static_cast<T>(0)-1; //Should wrap around to produce EOF/Out-of-space value

        static constexpr unsigned long DEFAULT_TABLE_SIZE = 100;

    public:
        EncodeDictionary(void);
        explicit EncodeDictionary(unsigned long initialSize);
        EncodeDictionary(unsigned long initialSize,bool autoRehash);
        ~EncodeDictionary(void);

        T add(const std::vector<unsigned char>& key);

        bool inDictionary(const unsigned char& byte) const;
        bool inDictionary(const std::vector<unsigned char>& vector) const;

        T getCode(const unsigned char& inputByte) const;
        T getCode(const std::vector<unsigned char>& vector) const;

        bool dictionaryFull(void) const;
    };
}

//====================Implementation====================//
namespace Dictionary{
    template <typename T>
    EncodeDictionary<T>::EncodeDictionary():EncodeDictionary(DEFAULT_TABLE_SIZE,true){
    }

    template <typename T>
    EncodeDictionary<T>::EncodeDictionary(unsigned long initialSize):EncodeDictionary(initialSize,true){
    }

    template<typename T>
    EncodeDictionary<T>::EncodeDictionary(unsigned long initialSize,bool autoRehash):currentAssignValue(0){
        hashTable = new Hash::HashTable<std::vector<unsigned char>,T>(hashFunction,initialSize);
        hashTable->setAutoRehash(autoRehash);
        for(int i=0;i<256&&!dictionaryFull();i++){
            hashTable->insert(std::vector<unsigned char>({static_cast<unsigned char>(i)}),currentAssignValue);
            currentAssignValue++;
        }
    }

    template <typename T>
    EncodeDictionary<T>::~EncodeDictionary(){
        delete hashTable;
    }

    template <typename T>
    T EncodeDictionary<T>::add(const std::vector<unsigned char>& key){
        if(dictionaryFull()){
            throw std::overflow_error("Dictionary is full(E)");
        }

        if(hashTable->inTable(key)){
            return(getCode(key));
        }
        else{
            T value = currentAssignValue;
            hashTable->insert(key,value);
            currentAssignValue++;
            return(value);
        }
    }

    template <typename T>
    bool EncodeDictionary<T>::inDictionary(const unsigned char& byte) const{
        return(hashTable->inTable(std::vector<unsigned char>({byte})));
    }

    template <typename T>
    bool EncodeDictionary<T>::inDictionary(const std::vector<unsigned char>& vector) const{
        return(hashTable->inTable(vector));
    }

    template <typename T>
    T EncodeDictionary<T>::getCode(const unsigned char& inputByte) const{
            return(this->getCode({std::vector<unsigned char>({inputByte})}));
    }

    template <typename T>
    T EncodeDictionary<T>::getCode(const std::vector<unsigned char>& vector) const{
        if(!hashTable->inTable(vector)){
            throw std::runtime_error("Key does not exist in dictionary(E)");
        }
        else{
            return(hashTable->getData(vector));
        }
    }

    template <typename T>
    bool EncodeDictionary<T>::dictionaryFull() const{
        return(currentAssignValue >= endOfFile-1);
    }

    template <typename T>
    unsigned long EncodeDictionary<T>::hashFunction(const std::vector<unsigned char>& vector){
        unsigned long seed = vector.size();
        for(unsigned char i : vector)
        {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return (seed);
    }

}

#endif