#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdexcept>

namespace Hash{

    template<typename T,typename U> //Operators "==" and "!=" must be defined for whatever object type T is used as the key, and "=" for object type U
    class HashTable {
    private:
        struct element{
            T key;
            U data;
            element* next;
        };

        element** tableArray;
        unsigned long arrayLen;
        unsigned long numberOfEntries; //Used for determining the load factor
        bool autoRehash;
        element** lastSearch; //Used to accelerate getData when used immediately after a search

        static constexpr double MAX_LOAD_FACTOR = 0.75;
        static constexpr long DEFAULT_TABLE_SIZE = 100;

        unsigned long (*const hashFunction)(const T&);
        element** search(const T&);

        void rehash(void);

    public:
        explicit HashTable(unsigned long(*funcPtr)(const T&));
        HashTable(unsigned long(*funcPtr)(const T&),unsigned long table_size);
        ~HashTable(void);

        void insert(const T& key, const U& data);
        void overWrite(const T& key,const U& data);
        void remove(const T& key);
        bool inTable(const T& key);
        U getData(const T& key);

        double getLoadFactor(void) const;
        void setAutoRehash(bool yesNo); //Used primarily for testing, can be set to false to disable auto-resizing
    };
}



//====================Implementation====================//
namespace Hash{
    template <typename T,typename U>
    HashTable<T,U>::HashTable(unsigned long(*funcPtr)(const T&))
            :HashTable(funcPtr,DEFAULT_TABLE_SIZE){
    }

    template <typename T,typename U>
    HashTable<T,U>::HashTable(unsigned long(*funcPtr)(const T&),unsigned long table_size)
            :hashFunction(funcPtr),
            arrayLen(table_size),
            numberOfEntries(0),
            autoRehash(true),
            lastSearch(nullptr){
        tableArray = new element*[arrayLen]{nullptr}; //Fills array with nullptr
    }

    template <typename T, typename U>
    HashTable<T,U>::~HashTable(){
        for(int i=0;i<arrayLen;i++){
            element* current = tableArray[i];
            while(current != nullptr){
                element* nextItem = current->next;
                delete(current);
                current = nextItem;
            }
        }
        delete [] tableArray;
    }

    template <typename T,typename U>
    typename HashTable<T,U>::element** HashTable<T,U>::search(const T& key){
        if(lastSearch != nullptr && (*lastSearch) != nullptr && (*lastSearch)->key == key){
            //Accelerates searches if the same thing is searched for twice in a row
            return(lastSearch);
        }
        else {
            element** current = &tableArray[hashFunction(key)%arrayLen];
            while (current != nullptr && *current != nullptr && (*current)->key != key) {
                current = &(*current)->next;
            }
            lastSearch = (current == nullptr || *current == nullptr)?nullptr:current;
            return (lastSearch);
        }
    }

    template <typename T, typename U>
    bool HashTable<T,U>::inTable(const T& key){
        return (search(key) != nullptr);
    }

    template <typename T,typename U>
    void HashTable<T,U>::insert(const T& key,const U& data){
        unsigned long index = hashFunction(key)%arrayLen;
        element* toInsert = new element{key,data,tableArray[index]};
        tableArray[index] = toInsert;
        numberOfEntries++;
        while(autoRehash && getLoadFactor() > MAX_LOAD_FACTOR){
            rehash();
        }
    }

    template<typename T, typename U>
    void HashTable<T,U>::overWrite(const T& key,const U& data){
        if(inTable(key)){
            (*search(key))->data = data;
        }
        else{
            throw std::invalid_argument("Key does not exist in table");
        }
    }

    template <typename T,typename U>
    void HashTable<T,U>::remove(const T& key){
        element** current = search(key);
        lastSearch = nullptr; //Sets to nullptr because the location at the last search is being deleted
        if(current != nullptr && (*current) != nullptr){
            //If current does not return nullptr, then it means the element is in the table
            element* elementToRemove = *current;
            *current = (*current)->next;

            delete elementToRemove;
            numberOfEntries--;
        }
        //If the key does not exist in the table, do nothing
    }

    template <typename T,typename U>
    U HashTable<T,U>::getData(const T& key){
        element** current = search(key);
        if(current == nullptr || (*current) == nullptr){
            //If it returns nullptr, then an element with the given key does not exist in the hash table
            throw(std::runtime_error("Key does not exist in table"));
        }
        else{
            return (*current)->data;
        }
    }

    template<typename T, typename U>
    double HashTable<T,U>::getLoadFactor() const{
        return (static_cast<double>(numberOfEntries)/arrayLen);
    }

    template <typename T, typename U>
    void HashTable<T,U>::setAutoRehash(bool yesNo){
        autoRehash = yesNo;
        if(autoRehash){
            //If autoRehashing is enabled, perform rehashing until the load factor is less than 0.75
            while(getLoadFactor() > MAX_LOAD_FACTOR){
                rehash();
            }
        }
    }

    template <typename T, typename U>
    void HashTable<T,U>::rehash(){
        element** oldArray = tableArray;
        unsigned long oldSize = arrayLen;

        arrayLen = (arrayLen*2)+1;
        tableArray = new element*[arrayLen]{nullptr};

        for(int i=0;i<oldSize;i++){
            element* current = oldArray[i];
            while(current != nullptr){
                insert(current->key,current->data);
                element* nextItem = current->next;
                delete current;
                current = nextItem;
            }
        }
        delete [] oldArray;
    }

}

#endif